.PHONY: clean

default:
	g++ MultiProcesses.cpp -lpthread -o MultiProcesses.out
	./MultiProcesses.out
MultiProcesses.out: MultiProcesses.cpp
	g++ MultiProcesses.cpp -lpthread -o MultiProcesses.out
	./MultiProcesses.out

clean:
	rm *.out