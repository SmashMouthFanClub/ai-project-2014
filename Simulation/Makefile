CFLAGS = -Wall

all: main.o
	g++ $(CFLAGS) bin/main.o -o bin/sim

main.o: src/main.cpp
	g++ $(CFLAGS) -c src/main.cpp -o bin/main.o

clean:
	rm bin/sim bin/*.o