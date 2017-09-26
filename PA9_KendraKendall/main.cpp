#include "worldsHardestGame.h"

int main(void)
{
	worldsHardestGame game;
	int choice = 0;

	do
	{
		system("cls");
		game.showMenu();
		choice = game.getChoice();
		switch (choice)
		{
		case 1:
			game.runGame();
			break;
		case 2:
			game.printInstructions();
			break;
		}
		system("pause");
		game.displayTotalDeaths();
	} while (choice != 3);

	return 0;
}