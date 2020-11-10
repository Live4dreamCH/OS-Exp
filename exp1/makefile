.PHONY: clean all compare

all: MultiProcesses.out MultiThreads.out MultiProcesses_fib.out

compare: MultiThreads.out MultiProcesses_fib.out

default: all

MultiProcesses.out: MultiProcesses.cpp
	g++ MultiProcesses.cpp -o MultiProcesses.out -Wall
	./MultiProcesses.out

MultiThreads.out: MultiThreads.cpp
	g++ MultiThreads.cpp -pthread -o MultiThreads.out -Wall
	time -v ./MultiThreads.out

pointer.out: pointer.cpp
	g++ pointer.cpp -o pointer.out -Wall -ggdb
	gdb pointer.out

MultiProcesses_fib.out: MultiProcesses_fib.cpp
	g++ MultiProcesses_fib.cpp -o MultiProcesses_fib.out -Wall
	time -v ./MultiProcesses_fib.out

clean:
	rm *.out