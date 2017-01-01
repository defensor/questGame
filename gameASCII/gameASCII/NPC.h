#pragma once

#include <vector>
#include "Item.h"
#include "Player.h"

using namespace std;

class Player;
class Item;
typedef vector<Item> Items;

class NPC
{
public:
	NPC(char tile);

	void getPosition(int & x, int & y){ x = _x; y = _y; };
	char getTile()const{ return _tile; }
	string getName()const{ return _name; }

	Item gettingItem(short id);
	bool isItem(short id) const;
	void initDialog(Player & player);
	void initSellerDialog(Player & player);
	void initSmithDialog(Player & player);
	void showSellerDialog(unsigned money);
	void showSmithDialog(Items items, unsigned money);

	void setPosition(int x, int y);
private:
	int _x;
	int _y;
	char _tile;
	string _name;

public:
	vector<Item> _items; //Товары
};