#include "main.h"

int main(void) {

	enum GAME_MODE { STARRT = 0,MENU,OPTION,END };
	std::cout << "Beta!" << '\n';

	int key = 0;

	while (key != END)
	{
		std::cout << "MENU =1\nOPTION = 2\nEND = 3" << '\n';
		std::cin >> key;

		switch (key)
		{
		case MENU:
			menu();
			break;
		case END:
			std::cout << "Exit";
			std::exit(0);
			break;


		default:
			std::cout << "MENU =1\nOPTION = 2\nEND = 3" << '\n';
			std::cin >> key;
			break;
		}
	}

	return 0;
}

void menu(void) {
	std::cout << "Menu";
}	