#include "Enemy.h"
#include <random>
#include <ctime>


Enemy::Enemy(string name, char tile, int level, int attack, int defense, int health, int xp, int money)
{
	_name = name;
	_tile = tile;
	_level = level;
	_attack = attack;
	_defense = defense;
	_health = health;
	_experienceValue = xp;
	_money = money;
}

//Sets the position of the enemy
void Enemy::setPosition(int x, int y)
{
	_x = x;
	_y = y;
}

//Gets the position of the enemy using reference variable
void Enemy::getPosition(int &x, int &y) const
{
	x = _x;
	y = _y;
}

int Enemy::attack()
{
	static default_random_engine randomEngine(unsigned(time(NULL)));
	uniform_int_distribution<int> attackRoll(0, _attack);

	return attackRoll(randomEngine);
}

int Enemy::takeDamage(int attack)
{
	attack -= _defense;

	//check if the atack does damage
	if (attack > 0)
	{
		_health -= attack;
		//check if he died
		if (_health <= 0)
		{
			return _experienceValue;
		}
	}
	return 0;
}

char Enemy::getMove(int playerX, int playerY)
{
	static default_random_engine randomEngine(unsigned(time(NULL)));
	uniform_int_distribution<int> moveRoll(0, 7);

	int distance;
	int dx = _x - playerX;
	int dy = _y - playerY;
	int adx = abs(dx);
	int ady = abs(dy);

	distance = adx + ady;


	//����� �������� ��������� ���������� ����������� �������� �� ������ � ������ ���� �� ����� ��������..................................................
	if (distance <= 5)
	{
		/*
		short localMap[11][11];
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if ((_x + (j - 5) < 0) || (_y + (i - 5) < 0))
				{
					localMap[i][j] = -1;
					continue;
				}
				if (getTile(j, i))
				localMap[i][j] = 0;
			}
		}
		*/

		//Moving along X axis
		if (adx > ady)
		{
			if (dx > 0)
				return 'a';
			else
				return 'd';
		}
		else //Move along Y axis
		{
			if (dy > 0)
				return 'w';
			else
				return 's';
		}
	}

	if (route.empty())
	{
		int direct = moveRoll(randomEngine); // Movings direct
		srand(unsigned(time(NULL)));
		int dist = rand() % 4 + 1; // Movings distance

		char step;
		
		switch (direct)
		{
		case 0:
			step = 'a';
			break;
		case 1:
			step = 'w';
			break;
		case 2:
			step = 's';
			break;
		case 3:
			step = 'd';
			break;
		default:
			step = '.';
		}

		for (int i = 0; i < dist; i++)
			route.push(step);
	}

	char step = route.top();
	route.pop();
	
	return step;
}