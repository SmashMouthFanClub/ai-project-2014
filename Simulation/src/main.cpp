#include <iostream>
#include "Game.h"

int main(int argc, char **argv) {
	Game a(400, 300);

	std::cout << a.GetViewWidth() << std::endl;

	while (a.Tick());

	return 0;
}