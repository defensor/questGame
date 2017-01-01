#include "GameSystem.h"
#include <iostream>
#include <conio.h>

//Constructor sets up the game
GameSystem::GameSystem(string levelFileName)
{
	_player.init(1, 100, 10, 0, 0, 0);
	_level.load(levelFileName, _player);

	system("PAUSE");
}

void GameSystem::PlayGame()
{
	while (_level.getNumsOfEnemies() > 0)
	{
		_level.print(_player);
		_level.updateEnemies(_player);
		playerMove();	
	}
	printf("\nYou WIN!!!\n");
}

void GameSystem::playerMove()
{
	char input;
	printf("Enter a move command(w/a/s/d): ");
	
	input = _getch();

	_level.movePlayer(input, _player);
}