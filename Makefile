webServer:src/main.o src/http_conn.o
	g++ -pthread src/main.o src/http_conn.o -o webServer
src/main.o:src/main.cpp src/locker.h src/http_conn.h src/threadpool.h
	g++ -c src/main.cpp -o src/main.o
src/http_conn.o:src/http_conn.cpp src/http_conn.h 
	g++ -c src/http_conn.cpp -o src/http_conn.o
