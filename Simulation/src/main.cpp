#include <iostream>
#include "Game.h"

int main(int argc, char **argv) {
	Game a(800, 600);

	while (a.Tick());

	return 0;
}