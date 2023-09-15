# Makefile for testing functions submitted with a main function
# It uses catch testing library
# Compile your program: make
# Run your program:  ./a.out
#

#Variables
CFLAGS = -g -Wall -std=c++11
CC = g++

# Add rules to compile your program.
	
a.out: huffman.cc node.h
	$(CC) $(CFLAGS) huffman.cc -o a.out

clean:
	rm -rf a.out *.o _TEST/NO_MAIN.cc _TEST/run_tests
