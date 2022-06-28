#include "main.h"

Menu menu;

int backgroundimage,pauseimage;
int catimage, appleimages;
int key = 0;

int Oldkey, Nowkey, KeyFlg;

void DrawScoreArea(void);

void DrawScoreArea(void)
{
	//ステージ画像表示
	//描画可能エリアを設定
	SetDrawArea(0, 0, 500, 480);

	//エリアを残す
	SetDrawArea(0, 0, 640, 480);

	//スコア等表示領域
	DrawBox(500, 0, 640, 480, 0x009900, TRUE);

	//スコア等をを表示
	SetFontSize(16);
	DrawFormatString(510, 20, 0x000000, "ハイスコア");
	DrawFormatString(560, 40, 0xFFFFFF, "%08d", g_Ranking[0].score);
	DrawFormatString(510, 80, 0x000000, "採った数");
	DrawRotaGraph(523, 120, 0.3f, 0, g_Teki[0], TRUE, FALSE);
	DrawRotaGraph(573, 120, 0.3f, 0, g_Teki[1], TRUE, FALSE);
	DrawRotaGraph(623, 120, 0.3f, 0, g_Teki[2], TRUE, FALSE);

	DrawFormatString(510, 140, 0xFFFFFF, "%03d", g_EnemyCount1);
	DrawFormatString(560, 140, 0xFFFFFF, "%03d", g_EnemyCount2);
	DrawFormatString(610, 140, 0xFFFFFF, "%03d", g_EnemyCount3);
}

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