# TinyWebServer
Linux下轻量级Web服务器
```
## 实现功能
1. 使用线程池实现多线程并发；
2. 采用epoll+Reactor进行并发HTTP请求的处理；
3. 使用有限状态机思想来解析HTTP请求，目前支持GET请求、IPv4网络及HTTP1.1协议。
```

## 编译和使用
1. 使用git clone命令将仓库克隆到本地；
2. 进入仓库主目录执行make命令；
3. 生成的webServer即为可执行文件，该程序接收ip地址和端口号两个参数，使用./webServer [要绑定的ip地址] [监听端口号]执行程序。
