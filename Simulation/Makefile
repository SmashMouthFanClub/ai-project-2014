CC = g++ -std=c++11 -I include
CFLAGS = -Wall -g
LINK = -lOgreMain -lOIS -lboost_system -lode -lpthread

all: bin/main.o bin/Game.o bin/GameWorld.o bin/GameObject.o bin/AStarMap.o bin/StaticMesh.o bin/QLearningAgent.o bin/QLearningManager.o bin/GameState.o bin/ReflexAgent.o
	$(CC) $(CFLAGS) bin/*.o -o bin/Simulation $(LINK)

bin/main.o: src/main.cpp include/Game.h
	$(CC) $(CFLAGS) -c src/main.cpp -o bin/main.o

bin/Game.o: src/Game.cpp include/Game.h include/GameWorld.h
	$(CC) $(CFLAGS) -c src/Game.cpp -o bin/Game.o

bin/GameWorld.o: src/GameWorld.cpp include/GameWorld.h include/GameObject.h include/Game.h
	$(CC) $(CFLAGS) -c src/GameWorld.cpp -o bin/GameWorld.o

bin/GameObject.o: src/GameObject.cpp include/GameObject.h
	$(CC) $(CFLAGS) -c src/GameObject.cpp -o bin/GameObject.o

bin/AStarMap.o: src/AStarMap.cpp include/AStarMap.h include/GameState.h
	$(CC) $(CFLAGS) -c src/AStarMap.cpp -o bin/AStarMap.o

bin/QLearningAgent.o: src/QLearningAgent.cpp include/QLearningAgent.h include/GameState.h
	$(CC) $(CFLAGS) -c src/QLearningAgent.cpp -o bin/QLearningAgent.o

bin/QLearningManager.o: src/QLearningManager.cpp include/QLearningManager.h
	$(CC) $(CFLAGS) -c src/QLearningManager.cpp -o bin/QLearningManager.o

bin/ReflexAgent.o: src/ReflexAgent.cpp include/ReflexAgent.h include/GameState.h
	$(CC) $(CFLAGS) -c src/ReflexAgent.cpp -o bin/ReflexAgent.o

bin/GameState.o: src/GameState.cpp
	$(CC) $(CFLAGS) -c src/GameState.cpp -o bin/GameState.o

bin/StaticMesh.o: src/StaticMesh.cpp include/StaticMesh.h
	$(CC) $(CFLAGS) -c src/StaticMesh.cpp -o bin/StaticMesh.o

clean:
	rm -f bin/Simulation bin/*.o
