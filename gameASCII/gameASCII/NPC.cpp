#include <iostream>
#include "NPC.h"
#include "Level.h"


NPC::NPC(char tile) :
_tile(tile)
{
	switch (this->_tile)
	{
	case SELLER_TILE: this->_name = "Seller";
		break;
	case SMITH_TILE: this->_name = "Koval";
		break;
	}
}

void NPC::setPosition(int x, int y)
{
	this->_x = x;
	this->_y = y;
}

Item NPC::extractItem(short id)
{
	Item item = this->_items.at(id);
	this->_items[id] = this->_items.back();
	this->_items.pop_back();
	return item;
}

bool NPC::isItem(short id) const
{
	return (id >= 0) && (id < _items.size());
}

void NPC::showSellerDialog(unsigned money)
{
	int itemsNum = this->_items.size();
	system("CLS");
	printf("Money: %u\n", money);
	printf("%s\n\n", this->_name);
	printf("%s have %u items.\n", this->_name, itemsNum);
	if (itemsNum > 0)
		printf("\tNAME\tPRICE\tDMG\tDEF\n");
	for (int j = 0; j < itemsNum; j++)
		printf("%d.\t%s\t%d\t%d%%\t%d%%\n", j + 1, this->_items.at(j).getName(), this->_items.at(j).getPrice(), this->_items.at(j).getDamage() * 100, this->_items.at(j).getDefense() * 100);
	printf("0. Exit.");

}

void NPC::showSmithDialog(Items items, unsigned money)
{
	int itemVal = items.size();
	int count = 0;

	system("CLS");
	printf("Money: %u", money);
	printf("%s\n\n", this->_name);
	printf("%s can upgrade your items.\n", this->_name);

	bool isUpgradebleItem = false;
	for (int i = 0; i < itemVal && !isUpgradebleItem; i++)
		if (items[i].getLevel() < 10)
			isUpgradebleItem = true;

	if (isUpgradebleItem)
		printf("\tNAME\tLEVEL\tPRICE\tCUR DMG\tCUR DEF\tUP DMG\tUP DEF\n");
	for (vector<Item>::const_iterator ci = items.begin(); ci < items.end(); ci++)
		if (ci->getLevel() < 10)
			printf("%d.\t%s\t%u\t%u\t%u%%\t%u%%\t%u%%\t%u%%\n", ++count, ci->getName(), ci->getLevel(), ci->getUpdatePrice(), ci->getDamage() * 100, ci->getDefense() * 100, (ci->getDamage() + ci->getDamageUpdateDifference()) * 100, (ci->getDefense() + ci->getDefenceUpdateDifference()) * 100);
	printf("0. Exit.");
}

void NPC::initSellerDialog(Player & player)
{
	this->showSellerDialog(player.getMoney());

	char choice = ' ';
	while (true)
	{
		choice = getchar();

		if (choice == '0')
			break;

		if (player.buyItem(*this, int(choice) - 48))
			this->showSellerDialog(player.getMoney());

		player.reloadItem();
	}
}

void NPC::initSmithDialog(Player & player)
{
	this->showSmithDialog(player._items, player.getMoney());

	char choice = ' ';
	while (true)
	{
		choice = getchar();

		if (choice == '0')
			break;

		choice -= '1';

		if (!player.isItem(choice))
			continue;

		int id = 0;
		int i;
		for (i = 0; i < player._items.size(); i++)
		{
			if (player._items[i].getLevel() == 10)
				continue;
			if (id == choice)
				break;
			id++;
		}

		if (i == player._items.size())
			continue;

		Item & item = player._items.at(i);

		if (player.upgradeItem(item))
			this->showSmithDialog(player._items, player.getMoney());

		player.reloadItem();
	}
}

void NPC::initDialog(Player & player)
{
	switch (this->_tile)
	{
	case SELLER_TILE:
		this->initSellerDialog(player);
		break;
	case SMITH_TILE:
		this->initSmithDialog(player);
		break;
	}
}

