all: main

main:
	$(CXX) main.cpp -std=c++11 -Wall -O3
	# ./a
	# sh plot.sh

plot:
	./a
	sh plot.sh

#   for assembler
# 	g++ main.cpp \
		-S -g -masm=intel \
		-std=c++11 -Wall -O3 \
		-mavx \
		-fopenmp
#	./a.out
