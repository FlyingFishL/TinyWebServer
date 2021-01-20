#ifndef HEEPCONNECTION_H
#define HTTPCONNECTION_H
//http连接处理头文件
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<sys/uio.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<sys/stat.h>
#include<string.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<stdarg.h>
#include<errno.h>
#include"locker.h"

class http_conn
{
public:
	static const int FILENAME_LEN = 200;      //文件名的最大长度
	static const int READ_BUFFER_SIZE = 2048; //读缓冲区大小
	static const int WRITE_BUFFER_SIZE = 1024;//写缓冲区大小
	enum METHOD { GET = 0, POST, HEAD, PUT, DELETE, TRACE, OPTIONS, CONNECT, PATCH };//HTTP请求方法
	enum CHECK_STATE { CHECK_STATE_REQUESTLINE = 0, CHECK_STATE_HEADER, CHECK_STATE_CONTENT }; //主状态机所处状态
	enum LINE_STATUS { LINE_OK = 0, LINE_BAD, LINE_OPEN }; //行读取状态即从状态机状态
	enum HTTP_CODE { NO_REQUEST, GET_REQUEST, BAD_REQUEST, NO_RESOURCE, FORBIDDEN_REQUEST, FILE_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION };   //http请求结果

	http_conn(){}
	~http_conn(){}

	void init(int sockfd, const sockaddr_in& addr);	  //初始化新接受的连接
	void close_conn(bool real_close = true);         //关闭连接
	void process();                                  //处理客户请求
	bool read();                                     //非阻塞读操作
	bool write();                                  //非阻塞写操作

	static int m_epollfd;                          //epoll文件描述符
	static int m_user_count;                       //用户数量
private:
	void init();                                //初始化连接
	HTTP_CODE process_read();                   //解析http请求
	bool process_write(HTTP_CODE ret);          //填充http应答

	//下面这一组函数将被process_read调用以分析http请求
	HTTP_CODE parse_request_line(char* text);
	HTTP_CODE parse_headers(char* text);
	HTTP_CODE parse_content(char* text);
	HTTP_CODE do_request();
	char* get_line() { return m_read_buf + m_start_line; }
	LINE_STATUS parse_line();

	//下面这一组函数将被process_write调用以填充http应答
	void unmap();
	bool add_response(const char* formant, ...);
	bool add_content(const char* content);
	bool add_status_line(int status, const char* title);
	bool add_headers(int content_length);
	bool add_content_length(int content_length);
	bool add_linger();
	bool add_blank_line();

	//该HTTP连接的socket和对方的socket地址
	int m_sockfd;
	sockaddr_in m_address;

	//读缓冲区
	char m_read_buf[READ_BUFFER_SIZE];
	int m_read_idx;                 //已读入数据的最后一个字节的下一个位置
	int m_checked_idx;             //正在分析的字符在读缓冲区的位置
	int m_start_line;              //正在解析的行的起始位置
	//写缓存区
	char m_write_buf[WRITE_BUFFER_SIZE];   
	int m_write_idx;             //写缓冲区中待发送的字节数

	CHECK_STATE m_check_state;      //主状态机所处状态
	METHOD m_method;                //请求方法

	char m_real_file[FILENAME_LEN];    //用户请求的目标文件的完整路径
	char* m_url;                       //用户请求的目标文件的文件名
	char* m_version;                  //HTTP协议版本号
	char* m_host;                     //主机名
	int m_content_length;              //http请求消息体的长度
	bool m_linger;                     //是否保存连接

	char* m_file_address;               //用户请求的目标文件被mmap到内存中的起始位置
	struct stat m_file_stat;            //目标文件的状态
	//使用writev来执行写操作，所以定义下面两个成员，其中m_iv_count表示写内存块的数量
	struct iovec m_iv[2];     
	int m_iv_count;
};
#endif