#pragma once

#include <iostream>
#include <stack>

using namespace std;

class Enemy
{
public:
	Enemy(string name, char tile, int level, int attack, int defense, int health, int xp, int money);

	int attack();

	int takeDamage(int attack);

	// Setters
	void setPosition(int x, int y);

	// Getters
	void getPosition(int &x, int &y)const;
	string getName()const{ return _name; }
	char getTile()const{ return _tile; }
	int getMoney()const{ return _money; }

	// Gets AI move command
	char getMove(int playerX, int playerY);

private:
	// Characters
	string _name;
	char _tile;

	int _level;
	int _attack;
	int _defense;
	int _health;
	int _experienceValue;
	int _money;

	// Route
	stack<char> route;

	//Position
	int _x;
	int _y;
};