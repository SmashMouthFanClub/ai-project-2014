#include "Game.h"
#include "GameWorld.h"

GameWorld::GameWorld(Game& game) : m_game(game) {

}

bool GameWorld::Update() {
	return true;
}