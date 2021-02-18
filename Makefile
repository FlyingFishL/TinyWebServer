bin/webServer:bin/main.o bin/http_conn.o
	g++ bin/mian.o bin/http_conn.o -o bin.webServer
bin/main.o:src/main.cpp src/locker.h src/http_conn.h src/threadpool.h
	g++ -pthread -std=c++11 src/main.cpp -o bin/main.o
bin/http_conn.o:src/http_conn.cpp src/http_conn.h src/locker.h 
	g++ -pthread -std=c++11 src/http_conn.cpp -o bin/http_conn.o
