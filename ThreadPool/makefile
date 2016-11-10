OBJ=main.o CThread.o ThreadPool.o CTask.o
CC=g++
CFGFLAGS= -g -Wall -lpthread

ThreadPool:$(OBJ)
	$(CC) -o ThreadPool $(CFGFLAGS) $(OBJ) 
main.o:main.cpp
	g++ -c -g main.cpp
CThread.o:CThread.cpp
	g++ -c -g CThread.cpp
ThreadPool.o:ThreadPool.cpp
	g++ -c -g ThreadPool.cpp
CTask.o:CTask.cpp
	g++ -c -g CTask.cpp

clean:
	rm -rf ThreadPool 