#pragma once

#include <string>
#include <vector>
#include "Enemy.h"
#include "Player.h"
#include "NPC.h"


using namespace std;

enum {
	PLAYER_TILE = '@', STONE_TILE = '#', VDOOR_TILE = '|', HDOOR_TILE = '-', GOBLIN_TILE = 'g', SNAKE_TILE = 's', BANDIT_TILE = 'B', OGRE_TILE = 'O', GOLEM_TILE = 'G', DRAGON_TILE = 'D', SMITH_TILE = '&', SELLER_TILE = '$', SPACE_TILE = '.'
};

class Level
{
public:
	Level();

	void load(string fileName, Player &player);

	void print(Player player);
	void movePlayer(char input, Player &player);
	void updateEnemies(Player &player);
	//Getters
	char getTile(int x, int y);
	int getNumsOfEnemies() { return _enemies.size(); }
	//Setters
	void setTile(int x, int y, char tile);

private:
	void processPlayerMove(Player &player, int targetX, int targetY);
	void processEnemyMove(Player &player, int enemyIndex, int targetX, int targetY);
	void battleMonster(Player &player, int targetX, int targetY);
	void processNPCDialog(Player &player, int targetX, int targetY);
	void openTheDoor(int x, int y);
private:
	vector<string> _levelData;
	vector<Enemy> _enemies;
	vector<NPC> _NPC;
};