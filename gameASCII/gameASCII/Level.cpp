#include "Level.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <conio.h>
#include <ctime>

Level::Level()
{

}

void Level::load(string fileName, Player &player)
{
	//

	ifstream file;

	file.open(fileName);
	if (file.fail())
	{
		perror(fileName.c_str());
		system("PAUSE");
		exit(1);
	}

	string line;

	while (getline(file, line))
	{
		_levelData.push_back(line);
	}

	file.close();

	//Process the level
	char tile;

	for (int i = 0; i < _levelData.size(); i++)
		for (int j = 0; j < _levelData[i].size(); j++)
		{
			tile = _levelData[i][j];
			
			switch (tile)
			{
			case PLAYER_TILE: //Player
				player.setPosition(j, i);
				break;
			case SELLER_TILE: //Seller
				_NPC.push_back(NPC(tile));
				_NPC.back().setPosition(j, i);
				_NPC.back()._items.push_back(Item("Armor", 70, 5, 0.2, 0, 1, 0.03, 0));
				_NPC.back()._items.push_back(Item("Sword", 50, 5, 0, 0.2, 1, 0, 0.03));
				break;
			case SMITH_TILE: //Koval
				_NPC.push_back(NPC(tile));
				_NPC.back().setPosition(j, i);
				break;
			case SNAKE_TILE: //Snake
				_enemies.push_back(Enemy("Snake", tile, 1, 3, 1, 10, 50, 10));
				_enemies.back().setPosition(j, i);
				break;
			case GOBLIN_TILE: //Goblin
				_enemies.push_back(Enemy("Goblin", tile, 2, 10, 3, 35, 150, 15));
				_enemies.back().setPosition(j, i);
				break;
			case BANDIT_TILE: //Bandit
				_enemies.push_back(Enemy("Bandit", tile, 3, 15, 10, 100, 250, 30));
				_enemies.back().setPosition(j, i);
				break;
			case OGRE_TILE: //Ogre
				_enemies.push_back(Enemy("Ogre", tile, 4, 40, 40, 200, 500, 50));
				_enemies.back().setPosition(j, i);
				break;
			case GOLEM_TILE: //Golem
				_enemies.push_back(Enemy("Golem", tile, 20, 150, 50, 500, 1000, 100));
				_enemies.back().setPosition(j, i);
				break;
			case DRAGON_TILE: //Dragon
				_enemies.push_back(Enemy("Dragon", tile, 100, 2000, 2000, 2000, 99999, 1000));
				_enemies.back().setPosition(j, i);
				break;
			}
		}
}

void Level::print(Player player)
{
	system("CLS");
	printf("Player: %d/%d XP  %dlvl\n", player.getExperience(), player.getNextLevelXP(), player.getLevel());
	printf("HP: %d/%d\n", player.getHealth(), player.getMaxHealt());
	printf("Attack: %d\n", player.getAttack());
	printf("Defense: %d%\n", (int)(player.getDefense() * 100));
	printf("Money: %d\n", player.getMoney());

	for (vector<string>::const_iterator ci = _levelData.begin(); ci < _levelData.end(); ci++)
		printf("%s\n", ci->c_str());
	
}

void Level::movePlayer(char input, Player &player)
{
	int playerX;
	int playerY;

	player.getPosition(playerX, playerY);


	switch (input)
	{
	case 'w': //up
	case 'W':
		processPlayerMove(player, playerX, playerY - 1);
		break;
	case 's': //down
	case 'S':
		processPlayerMove(player, playerX, playerY + 1);
		break;
	case 'a': //left
	case 'A':
		processPlayerMove(player, playerX - 1, playerY);
		break;
	case 'd': //right
	case 'D':
		processPlayerMove(player, playerX + 1, playerY);
		break;
	default:
		break;
	}
}

char Level::getTile(int x, int y)
{
	return _levelData[y][x];
}

void Level::setTile(int x, int y, char tile)
{
	_levelData[y][x] = tile;
}

void Level::processPlayerMove(Player &player, int targetX, int targetY)
{
	int playerX;
	int playerY;

	player.getPosition(playerX, playerY);

	char moveTile = getTile(targetX, targetY);

	switch (moveTile)
	{
	case '.':
		player.setPosition(targetX, targetY);
		setTile(playerX, playerY, SPACE_TILE);
		setTile(targetX, targetY, PLAYER_TILE);
		break;
	case STONE_TILE:
		break;
	case SELLER_TILE:
		//cout << "Pressed 'E' for dialog." << endl;
		processNPCDialog(player, targetX, targetY);
		break;
	case SMITH_TILE:
		processNPCDialog(player, targetX, targetY);
	case VDOOR_TILE:
	case HDOOR_TILE:
		openTheDoor(targetX, targetY);
		break;
	default:
		battleMonster(player, targetX, targetY);
		break;
	}
}

void Level::battleMonster(Player &player, int targetX, int targetY)
{
	int enemyX;
	int enemyY;
	int playerX;
	int playerY;
	int attackRoll;
	int attackResult;
	int money;
	string enemyName;

	player.getPosition(playerX, playerY);

	for (int i = 0; i < _enemies.size(); i++)
	{
		_enemies[i].getPosition(enemyX, enemyY);
		enemyName = _enemies[i].getName();

		if ((enemyX == targetX) && (enemyY == targetY))
		{
			print(player);

			//Battle!
			attackRoll = player.attack();
			printf("\nPlayer attacked %s with a roll of %d.\n", enemyName.c_str(), attackRoll);
			attackResult = _enemies[i].takeDamage(attackRoll);
			
			srand(unsigned(time(NULL)));
			
			if (attackResult != 0)
			{
				money = _enemies[i].getMoney() * (rand() % 8 + 2) / 10;
				setTile(targetX, targetY, '.');
				print(player);
				printf("Monster died!\n");
				//Remove the enemy
				_enemies[i] = _enemies.back();
				_enemies.pop_back();
				i--;
				player.addExperience(attackResult);
				player.addMoney(money);
		
				return;
			}

			//Monster turns!
			attackRoll = _enemies[i].attack();
			printf("%s attacked player with a roll of %d.\n", enemyName.c_str(), attackRoll);
			attackResult = player.takeDamage(attackRoll);

			if (attackResult != 0)
			{
				setTile(playerX, playerY, 'X');
				print(player);
				printf("You died!\n");
				system("PAUSE");

				exit(0);
			}
			system("PAUSE");
			return;
		}

	}
}

void Level::updateEnemies(Player &player)
{
	char aiMove;
	int playerX;
	int playerY;
	int enemyX;
	int enemyY;

	player.getPosition(playerX, playerY);
	for (int i = 0; i < _enemies.size(); i++)
	{
		_enemies[i].getPosition(enemyX, enemyY);
		aiMove = _enemies[i].getMove(playerX, playerY);

		switch (aiMove)
		{
		case 'ö':
		case 'w': //up
			processEnemyMove(player, i, enemyX, enemyY - 1);
			break;
		case 'û':
		case 's': //down
			processEnemyMove(player, i, enemyX, enemyY + 1);
			break;
		case 'ô':
		case 'a': //left
			processEnemyMove(player, i, enemyX - 1, enemyY);
			break;
		case 'â':
		case 'd': //right
			processEnemyMove(player, i, enemyX + 1, enemyY);
			break;
		}
	}
}

void Level::processEnemyMove(Player &player, int enemyIndex, int targetX, int targetY)
{
	int playerX;
	int playerY;
	int enemyX;
	int enemyY;

	_enemies[enemyIndex].getPosition(enemyX, enemyY);
	player.getPosition(playerX, playerY);

	char moveTile = getTile(targetX, targetY);

	switch (moveTile)
	{
	case SPACE_TILE:
		_enemies[enemyIndex].setPosition(targetX, targetY);
		setTile(enemyX, enemyY, SPACE_TILE);
		setTile(targetX, targetY, _enemies[enemyIndex].getTile());
		break;
	case PLAYER_TILE:
		battleMonster(player, enemyX, enemyY);
		break;
	default:
		break;
	}
}

void Level::processNPCDialog(Player &player, int targetX, int targetY)
{
	int npcX;
	int npcY;
	int playerX;
	int playerY;
	string npcName;

	player.getPosition(playerX, playerY);

	for (int i = 0; i < _NPC.size(); i++)
	{
		_NPC[i].getPosition(npcX, npcY);

		if ((npcX == targetX) && (npcY == targetY))
		{
			_NPC[i].initDialog(player);
			break;
		}
	}
}

void Level::openTheDoor(int x, int y)
{
	if (getTile(x, y) == HDOOR_TILE || getTile(x, y) == VDOOR_TILE)
		setTile(x, y, SPACE_TILE);
}