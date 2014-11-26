CC = g++ -std=c++11 -I include
CFLAGS = -Wall
LINK = -lOgreMain -lOIS -lboost_system

all: main.o Game.o GameWorld.o
	$(CC) $(CFLAGS) bin/*.o -o bin/Simulation $(LINK)

main.o: src/main.cpp
	$(CC) $(CFLAGS) -c src/main.cpp -o bin/main.o

Game.o: src/Game.cpp include/Game.h
	$(CC) $(CFLAGS) -c src/Game.cpp -o bin/Game.o

GameWorld.o: src/GameWorld.cpp include/GameWorld.h
	$(CC) $(CFLAGS) -c src/GameWorld.cpp -o bin/GameWorld.o

clean:
	rm -f bin/Simulation bin/*.o