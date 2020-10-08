OPT = -O2

all: benchmark

benchmark: benchmark.cpp
	g++ -Wall -pg -g $(OPT) benchmark.cpp -o benchmark

clean:
	rm -rf benchmark
