#include "main.h"

Menu menu;

int backgroundimage,pauseimage;
int catimage, appleimages;
int key = 0;

int Oldkey, Nowkey, KeyFlg;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdshow)
{
	SetMainWindowText("");
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1) return -1;
	if (menu.LoadImages() == -1) return -1;
	SetDrawScreen(DX_SCREEN_BACK);

	enum GAME_MODE { STARRT = 0, MENU, OPTION, END };

	std::cout << "Beta!" << '\n';

	while (ProcessMessage() == 0 && key != END && !(KeyFlg & PAD_INPUT_START))
	{
		std::cout << "MENU =1\nOPTION = 2\nEND = 3" << '\n' << std::flush;
		std::cin >> key;

		Oldkey = Nowkey;
		Nowkey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		KeyFlg = Nowkey & ~Oldkey;

		ClearDrawScreen();

		switch (key)
		{
		case MENU:
			menu.menu();
			break;
		case END:
			std::cout << "Exit";
			//std::exit(0);
			DxLib_End();
			break;


		default:
			std::cout << "MENU =1\nOPTION = 2\nEND = 3" << '\n' ;
			std::cin >> key;
			break;
		}
	}
	DxLib_End();

	return 0;
}

void Menu::menu(void) {

	DrawGraph(0, 0, backgroundimage, FALSE);
	std::cout << "Menu" << std::endl;
}	

void Menu::GameModeChange(int num) {
	try {
		if (0 <= num && 3 >= num) {
			switch (num) {
			case 1:
				break;
			}
		}if (num == key) {
			throw - 1;
		}
	}
	catch (int err) {
			std::cout << err << std::endl;
		}

}

int Menu::LoadImages(void) {
	if ((backgroundimage = LoadGraph("images/background.jpg")) == -1) return -1;
	if ((pauseimage = LoadGraph("images/pause.jpg")) == -1) return -1;

	return 0;
}