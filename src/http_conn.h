#ifndef HEEPCONNECTION_H
#define HTTPCONNECTION_H
//http���Ӵ���ͷ�ļ�
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
	static const int FILENAME_LEN = 200;      //�ļ�������󳤶�
	static const int READ_BUFFER_SIZE = 2048; //����������С
	static const int WRITE_BUFFER_SIZE = 1024;//д��������С
	enum METHOD { GET = 0, POST, HEAD, PUT, DELETE, TRACE, OPTIONS, CONNECT, PATCH };//HTTP���󷽷�
	enum CHECK_STATE { CHECK_STATE_REQUESTLINE = 0, CHECK_STATE_HEADER, CHECK_STATE_CONTENT }; //��״̬������״̬
	enum LINE_STATUS { LINE_OK = 0, LINE_BAD, LINE_OPEN }; //�ж�ȡ״̬����״̬��״̬
	enum HTTP_CODE { NO_REQUEST, GET_REQUEST, BAD_REQUEST, NO_RESOURCE, FORBIDDEN_REQUEST, FILE_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION };   //http������

	http_conn(){}
	~http_conn(){}

	void init(int sockfd, const sockaddr_in& addr);	  //��ʼ���½��ܵ�����
	void close_conn(bool real_close = true);         //�ر�����
	void process();                                  //����ͻ�����
	bool read();                                     //������������
	bool write();                                  //������д����

	static int m_epollfd;                          //epoll�ļ�������
	static int m_user_count;                       //�û�����
private:
	void init();                                //��ʼ������
	HTTP_CODE process_read();                   //����http����
	bool process_write(HTTP_CODE ret);          //���httpӦ��

	//������һ�麯������process_read�����Է���http����
	HTTP_CODE parse_request_line(char* text);
	HTTP_CODE parse_headers(char* text);
	HTTP_CODE parse_content(char* text);
	HTTP_CODE do_request();
	char* get_line() { return m_read_buf + m_start_line; }
	LINE_STATUS parse_line();

	//������һ�麯������process_write���������httpӦ��
	void unmap();
	bool add_response(const char* formant, ...);
	bool add_content(const char* content);
	bool add_status_line(int status, const char* title);
	bool add_headers(int content_length);
	bool add_content_length(int content_length);
	bool add_linger();
	bool add_blank_line();

	//��HTTP���ӵ�socket�ͶԷ���socket��ַ
	int m_sockfd;
	sockaddr_in m_address;

	//��������
	char m_read_buf[READ_BUFFER_SIZE];
	int m_read_idx;                 //�Ѷ������ݵ����һ���ֽڵ���һ��λ��
	int m_checked_idx;             //���ڷ������ַ��ڶ���������λ��
	int m_start_line;              //���ڽ������е���ʼλ��
	//д������
	char m_write_buf[WRITE_BUFFER_SIZE];   
	int m_write_idx;             //д�������д����͵��ֽ���

	CHECK_STATE m_check_state;      //��״̬������״̬
	METHOD m_method;                //���󷽷�

	char m_real_file[FILENAME_LEN];    //�û������Ŀ���ļ�������·��
	char* m_url;                       //�û������Ŀ���ļ����ļ���
	char* m_version;                  //HTTPЭ��汾��
	char* m_host;                     //������
	int m_content_length;              //http������Ϣ��ĳ���
	bool m_linger;                     //�Ƿ񱣴�����

	char* m_file_address;               //�û������Ŀ���ļ���mmap���ڴ��е���ʼλ��
	struct stat m_file_stat;            //Ŀ���ļ���״̬
	//ʹ��writev��ִ��д���������Զ�������������Ա������m_iv_count��ʾд�ڴ�������
	struct iovec m_iv[2];     
	int m_iv_count;
};
#endif