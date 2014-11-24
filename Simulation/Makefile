CC = g++ -std=c++11 -I include
CFLAGS = -Wall

all: main.o Simulation.o AStarMap.o
	$(CC) $(CFLAGS) bin/*.o -o bin/sim

main.o: src/main.cpp
	$(CC) $(CFLAGS) -c src/main.cpp -o bin/main.o

Simulation.o: src/Simulation.cpp include/Simulation.h
	$(CC) $(CFLAGS) -c src/Simulation.cpp -o bin/Simulation.o

AStarMap.o: src/AStarMap.cpp include/AStarMap.h
	$(CC) $(CFLAGS) -c src/AStarMap.cpp -o bin/AStarMap.o

clean:
	rm bin/sim bin/*.o