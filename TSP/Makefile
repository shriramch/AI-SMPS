# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -std=c++17 -g -pthread
 
# ****************************************************
# Targets needed to bring the executable up to date
 
main: main.o input.o crossover.o newGeneration.o runGenetic.o Graph.o random.o GraphTSP.hpp
	$(CC) $(CFLAGS) -o main main.o input.o crossover.o newGeneration.o runGenetic.o Graph.o random.o
 
# The main.o target can be written more simply

main.o: main.cpp GraphTSP.hpp
	$(CC) $(CFLAGS) -c main.cpp 

input.o: input.cpp GraphTSP.hpp
	$(CC) $(CFLAGS) -c input.cpp 

crossover.o: crossover.cpp GraphTSP.hpp
	$(CC) $(CFLAGS) -c crossover.cpp

newGeneration.o: newGeneration.cpp GraphTSP.hpp
	$(CC) $(CFLAGS) -c newGeneration.cpp  

runGenetic.o: runGenetic.cpp GraphTSP.hpp
	$(CC) $(CFLAGS) -c runGenetic.cpp

Graph.o: Graph.cpp GraphTSP.hpp
	$(CC) $(CFLAGS) -c Graph.cpp 

Random.o: random.cpp GraphTSP.hpp
	$(CC) $(CFLAGS) -c random.cpp

clean:
	rm *o main