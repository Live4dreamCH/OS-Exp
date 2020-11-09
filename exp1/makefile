.PHONY: clean all

all: MultiProcesses.out MultiThreads.out

default: all

MultiProcesses.out: MultiProcesses.cpp
	g++ MultiProcesses.cpp -o MultiProcesses.out -Wall
	./MultiProcesses.out

MultiThreads.out: MultiThreads.cpp
	g++ MultiThreads.cpp -pthread -o MultiThreads.out -Wall
	./MultiThreads.out

pointer.out: pointer.cpp
	g++ pointer.cpp -o pointer.out -Wall -ggdb
	gdb pointer.out

clean:
	rm *.out *.temp