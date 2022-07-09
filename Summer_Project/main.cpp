#include "main.h"
#include <string>	//rankingで使用

#define TIMELIMIT 1800 + 60

LPCSTR font_path = "../Fonts/jkmarugo/JK-Maru-Gothic-M.otf";

enum { DRAW_GAMETITLE, GAME_INIT, DRAW_RANKING, DRAW_HELP, DRAW_END, GAME_MAIN, INPUT_RANKING, END = 99 };
Apple apple[APPLE_MAX];
Apple AppleFunc;
int g_OldKey, g_NowKey, g_KeyFlg;
int MenuFont, PauseFont;
int apple_img[4];
int players_img[6];
int g_GameState = DRAW_GAMETITLE;
int g_Score = 0;
int timer;
int invincibletime;
int g_RankingImage;
bool StartFlg = false;
struct PLAYER g_player;
struct RankingData g_Ranking[RANKING_DATA];

int g_TitleImage;
int SelecterImage;
int g_PosY;
int JoyPadX, JoyPadY;
int player_angle = 1;
int SelectX, SelectY;
int PadTimer, PadSpeedTimer;
int g_WaitTime = 0;
int g_EndImage;
int g_StageImage;
bool apple_flg;
int apple_x;
int apple_y;
bool Pauseflg;
bool PadType = false;

int LoadImages();
int LoadSounds();
void DrawPause();

//サウンド用変数
int TitleBGM;
int GameMainBGM;
int RankingBGM;
int EndBGM;
int SE;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	SetMainWindowText("りんごおとし");
	ChangeWindowMode(TRUE);

	double dNextTime = GetNowCount();

	// フォント読み込み
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// フォント読込エラー処理
		MessageBox(NULL, "フォント読込失敗", "", MB_OK);
	}
	ChangeFont("JK丸ゴシック Medium", DX_CHARSET_DEFAULT);

	if (DxLib_Init() == -1)return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	if ((g_RankingImage = LoadGraph("images/Ranking.png")) == -1)return-1;
	if (LoadImages() == -1)return -1;
	if (LoadSounds() == -1)return -1;

	MenuFont = CreateFontToHandle("JK丸ゴシック Medium", 40, 1, DX_CHARSET_DEFAULT);
	PauseFont = CreateFontToHandle("JK丸ゴシック Medium", 80, 1, DX_CHARSET_DEFAULT);

	while (ProcessMessage() == 0 && g_GameState != END) {

		g_OldKey = g_NowKey;
		g_NowKey = GetJoypadInputState(DX_INPUT_PAD1);
		g_KeyFlg = g_NowKey & ~g_OldKey;

		GetJoypadAnalogInput(&JoyPadX, &JoyPadY, DX_INPUT_PAD1);
		if (CheckJoypadXInput(DX_INPUT_PAD1)) { PadType = true; }

		if (g_KeyFlg & (PadType ? 1024 : 4096)) g_GameState = END;
		SelectX = 0;
		SelectY = 0;

		if (++PadTimer > 10) {
			PadTimer = 0;
			if (JoyPadX > 800)SelectX = 1;
			if (JoyPadX < -800)SelectX = -1;
			if (JoyPadY > 800)SelectY = 1;
			if (JoyPadY < -800)SelectY = -1;
		}
		SetJoypadDeadZone(DX_INPUT_PAD1, 0.6f);


		ClearDrawScreen();

		switch (g_GameState)
		{
		case 0:
			DrawGameTitle();
			break;
		case 1:
			GameInit();
			break;
		case 2:
			DrawRanking();
			break;
		case 3:
			DrawHelp();
			break;
		case 4:
			DrawEnd();
			break;
		case 5:
			GameMain();
			break;
		case 6:
			InputRanking();
			break;
		}

		ScreenFlip();

		dNextTime += 16.66;
		if (dNextTime > GetNowCount()) {
			WaitTimer((int)dNextTime - GetNowCount());
		}

	}
	DxLib_End();

	return 0;
}


void DrawGameTitle(void) {
	static int MenuNo = 0;

	if (CheckSoundMem(RankingBGM) == 1)StopSoundMem(RankingBGM);
	if (CheckSoundMem(TitleBGM) == 0)PlaySoundMem(TitleBGM, DX_PLAYTYPE_BACK);

	if (SelectY == 1)if (++MenuNo > 3)MenuNo = 0;
	if (SelectY == -1)if (--MenuNo < 0)MenuNo = 3;
	if (g_KeyFlg & (PadType ? 16 : 32))g_GameState = MenuNo + 1;

	DrawGraph(0, 0, g_TitleImage, FALSE);
	static bool ani = true;
	if (ani == true) {
		static int aniy = rand() % 50;
		/*if(++aniy > 10)*/
		if (++g_WaitTime > 30) DrawFormatStringToHandle(200, 100 + aniy, 0x9c3e26, MenuFont, "り"); aniy = rand() % 20;
		if (g_WaitTime > 60) DrawFormatStringToHandle(250, 100 + aniy, 0x9c3e26, MenuFont, "ん"); aniy = rand() % 20;
		if (g_WaitTime > 120) DrawFormatStringToHandle(300, 100 + aniy, 0x9c3e26, MenuFont, "ご"); aniy = rand() % 20;
		if (g_WaitTime > 180) DrawFormatStringToHandle(350, 100 + aniy, 0x9c3e26, MenuFont, "お"); aniy = rand() % 20;
		if (g_WaitTime > 240) DrawFormatStringToHandle(400, 100 + aniy, 0x9c3e26, MenuFont, "と"); aniy = rand() % 20;
		if (g_WaitTime > 300) { DrawFormatStringToHandle(450, 100, 0x9c3e26, MenuFont, "し"); ani = false; g_WaitTime = 0; }
	}
	else if (ani == false) {

		DrawFormatStringToHandle(200, 100, 0x9c3e26, MenuFont, "り");
		DrawFormatStringToHandle(250, 100, 0x9c3e26, MenuFont, "ん");
		DrawFormatStringToHandle(300, 100, 0x9c3e26, MenuFont, "ご");
		DrawFormatStringToHandle(350, 100, 0x9c3e26, MenuFont, "お");
		DrawFormatStringToHandle(400, 100, 0x9c3e26, MenuFont, "と");
		DrawFormatStringToHandle(450, 100, 0x9c3e26, MenuFont, "し");
	}


	DrawFormatStringToHandle(420, 280, 0x9c3e26, MenuFont, "すたーと");
	DrawFormatStringToHandle(420, 320, 0x9c3e26, MenuFont, "らんきんぐ");
	DrawFormatStringToHandle(420, 360, 0x9c3e26, MenuFont, "へるぷ");
	DrawFormatStringToHandle(420, 400, 0x9c3e26, MenuFont, "えんど");

	DrawRotaGraph(400, 300 + MenuNo * 40, 1.0f, 0, SelecterImage, TRUE);
	//DrawRotaGraph(400, 300 + MenuNo * 40, 1.0f, M_PI / 2, SelecterImage, TRUE);
}

void GameInit(void) {
	g_Score = 0;
	StartFlg = true;
	timer = TIMELIMIT;
	invincibletime = 0;
	Pauseflg = false;

	for (int i = 0; i < 4; i++) {
		apple_count[i] = 0;
	}

	AppleFunc.AppleInit();

	//	プレイヤーの初期設定
	g_player.flg = TRUE;
	g_player.x = PLAYER_POS_X;
	g_player.y = PLAYER_POS_Y;
	g_player.w = PLAYER_WIDTH;
	g_player.h = PLAYER_HEIGHT;
	g_player.count = 0;
	g_player.speed = PLAYER_SPEED;
	g_player.Poisonflg = false;

	g_GameState = GAME_MAIN;
}

void DrawRanking(void) {

	if (CheckSoundMem(TitleBGM) == 1)StopSoundMem(TitleBGM);
	if (CheckSoundMem(GameMainBGM) == 1)StopSoundMem(GameMainBGM);
	if (CheckSoundMem(RankingBGM) == 0)PlaySoundMem(RankingBGM, DX_PLAYTYPE_BACK);

	if (g_KeyFlg & (PadType ? 32 : 64))g_GameState = DRAW_GAMETITLE;

	DrawGraph(0, 0, g_RankingImage, FALSE);

	for (int i = 0; i < RANKING_DATA; i++) {
		DrawFormatStringToHandle(50, 120 + i * 50, 0xffffff,MenuFont ,"%2d  %-10s  %10d", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
		
		if (++g_WaitTime < 120) {
			DrawString(150, 390, "---- Bボタンーをおしてタイトルへもどる ----", 0xffffff, 0);
		}
		else if (g_WaitTime > 360) { g_WaitTime = 0; }
	}
}

void DrawHelp(void) {
	if (g_KeyFlg & (PadType ? 32 : 64))	g_GameState = DRAW_GAMETITLE;
	if (g_KeyFlg & (PadType ? 16 : 32))	g_GameState = GAME_INIT;

	DrawGraph(0, 0, g_TitleImage, FALSE);

	DrawStringToHandle(260, 40, "へるぷ", 0xffffff, MenuFont, 0);

	DrawStringToHandle(20, 120, "このゲームは制限時間：３０秒で", 0xffffff, MenuFont, 0);
	DrawStringToHandle(20, 180, "おおくのりんごをとるゲームです。", 0xffffff, MenuFont, 0);
	DrawStringToHandle(20, 270, "B", 0x0000ff, MenuFont, 0);
	DrawStringToHandle(44, 270, "ボタンをおして", 0xffffff, MenuFont, 0);
	DrawStringToHandle(320, 270, "タイトルへもどる", 0xff8c00, MenuFont, 0);
	DrawStringToHandle(20, 330, "A", 0xff0000, MenuFont, 0);
	DrawStringToHandle(44, 330, "ボタンをおして", 0xffffff, MenuFont, 0);
	if (++g_WaitTime < 30) {
		DrawStringToHandle(320, 330, "ゲームスタート", 0xffff00, MenuFont, 0);
	}
	else if(g_WaitTime > 60){  g_WaitTime = 0; }
}

void DrawEnd(void) {
	int g_PosY = 0;

	if (CheckSoundMem(TitleBGM) == 1)StopSoundMem(TitleBGM);
	if (CheckSoundMem(EndBGM) == 0)PlaySoundMem(EndBGM, DX_PLAYTYPE_BACK);

	//エンド画像表示
	DrawGraph(0, 0, g_EndImage, FALSE);
	//エンディング表示
	if (++g_WaitTime < 600) { g_PosY = 300 - g_WaitTime / 2; }

	SetFontSize(24);
	DrawString(100, 170 + g_PosY, "タイトル　　　りんごのもり", 0xFFFFFF, 0);
	DrawString(100, 200 + g_PosY, "バージョン　　1.0", 0xFFFFFF, 0);
	DrawString(100, 230 + g_PosY, "最終更新日　　2022年6月28日", 0xFFFFFF, 0);
	DrawString(100, 260 + g_PosY, "制作者　　　　わん,ゆうた", 0xFFFFFF, 0);
	DrawString(100, 290 + g_PosY, "　　　　　　　しょうご,しき", 0xFFFFFF, 0);
	DrawString(100, 310 + g_PosY, "素材利用", 0xFFFFFF);
	DrawString(100, 340 + g_PosY, "　BGM　　,　 DOVA-SYNDROME", 0xFFFFFF, 0);
	DrawString(100, 365 + g_PosY, "　SE　　　　　効果音工房", 0xFFFFFF, 0);

	//タイムの加算処理＆終了
	if (++g_WaitTime > 900) { g_WaitTime = 0; g_GameState = END; }

	DeleteFontToHandle(MenuFont);
}

void GameMain(void) {
	if (CheckSoundMem(TitleBGM) == 1)StopSoundMem(TitleBGM);
	if (StartFlg == TRUE) {
		if (CheckSoundMem(GameMainBGM) == 0)PlaySoundMem(GameMainBGM, DX_PLAYTYPE_BACK, TRUE);
	}
	else {
		if (CheckSoundMem(GameMainBGM) == 0)PlaySoundMem(GameMainBGM, DX_PLAYTYPE_BACK, FALSE);
	}
	

	DrawGraph(0, 0, g_StageImage, FALSE);

	AppleFunc.AppleControl(Pauseflg);
	PlayerControl(Pauseflg);

	if (g_KeyFlg & (PadType ? 2048 : 8192)) {
		if (Pauseflg == false) {
			Pauseflg = true;
		}
		else {
			Pauseflg = false;
		}
	}

	//DrawFormatString(280, 250, 0x000000, "%d", Pauseflg);
	

	if (!Pauseflg) {

		if (CheckSoundMem(GameMainBGM) == 0)PlaySoundMem(GameMainBGM, DX_PLAYTYPE_BACK,FALSE);

		if (timer-- == 0) {
			if (g_Ranking[RANKING_DATA - 1].score >= g_Score) {
				g_GameState = DRAW_RANKING;
			}
			else {
				g_GameState = INPUT_RANKING;
			}
		}

	}
	else {
		DrawPause();
		StopSoundMem(GameMainBGM);
	}



	
}



int LoadImages() {
	if (LoadDivGraph("images/apple.png", 5, 4, 1, 50, 50, apple_img) == -1) return -1;

	if ((g_TitleImage = LoadGraph("images/Title.png")) == -1)return-1;
	if ((SelecterImage = LoadGraph("images/player2.png")) == -1)return-1;
	if ((g_EndImage = LoadGraph("images/background.png")) == -1)return-1;
	if (LoadDivGraph("images/apple.png", 4, 4, 1, 50, 50, apple_img) == -1)return -1;	//リンゴ
	if ((g_StageImage = LoadGraph("images/pause.png")) == -1)return-1;
	if (LoadDivGraph("images/player.png", 6, 3, 2, 32, 32, players_img) == -1)return -1;	//リンゴ
	return 0;
}

void InputRanking(void)
{
	if (CheckSoundMem(GameMainBGM) == 1)StopSoundMem(GameMainBGM);
	if (CheckSoundMem(RankingBGM) == 0)PlaySoundMem(RankingBGM, DX_PLAYTYPE_BACK);
	//ランキング画像表示
	DrawGraph(0, 0, g_RankingImage, FALSE);

	static char buf[10] = "_________";

	// 名前入力指示文字列の描画
	DrawFormatStringToHandle(120, 100, 0xFFFFFF, MenuFont, "ランキングに登録します");
	DrawFormatStringToHandle(65, 150, 0xFFFFFF, MenuFont, "名前を英字で入力してください");
	if (g_WaitTime < 30) {
		DrawString(180, 420, "---- STARTボタンをおして名前決定！ ----", 0xffffff, 0);
	}
	else if (g_WaitTime > 60) { g_WaitTime = 0; }
	// 名前の入力
	DrawFormatStringToHandle(120, 200, 0xFFFFFF, MenuFont, "> ");
	DrawBox(190, 200, 425, 250, 0x000055, TRUE);
	if (++g_WaitTime < 30) {
		DrawFormatStringToHandle(200, 205, 0xFFFFFF, MenuFont, "%s", buf);
	}
	else if (g_WaitTime > 60) { g_WaitTime = 0; }

	char input_char = '0';
	for (int j = 0; j < 5; j++) {
		for (int i = 1; i < 14; i++) {
			if (input_char < '{' || input_char < '[') {
				DrawFormatString(i * 30 + 100, 260 + 30 * j, 0x000000, "%c", input_char++);
			}
			if (input_char == ':') { input_char = 'a'; j = 1; i = 0; }
			if (input_char == '{') { input_char = 'A'; j = 3; i = 0; }
		}
	}
	static int selecterX = 0;
	static int selecterY = 0;
	if (SelectX == 1)if (++selecterX > 12)selecterX = 0;
	if (SelectX == -1)if (--selecterX < 0)selecterX = 12;
	if (SelectY == 1)if (++selecterY > 4)selecterY = 0;
	if (SelectY == -1)if (--selecterY < 0)selecterY = 4;
	if (g_KeyFlg & (PadType ? 16 : 32)) {}
	DrawBox(120 + 30 * selecterX, 250 + 30 * selecterY, 150 + 30 * selecterX, 280 + 30 * selecterY, 0x000000, FALSE);


	static int i = 0;
	if (i >= 9 || /*strlen(buf) >= 9 ||*/ g_KeyFlg & (PadType ? 2048 : 8192)) {
		buf[i] = '\0';
		std::string buf_str = buf;
		buf_str = buf_str.erase(i);
		strcpyDx(g_Ranking[RANKING_DATA - 1].name, buf_str.c_str());

		g_Ranking[RANKING_DATA- 1].score = g_Score;	// ランキングデータの１０番目にスコアを登録
		SortRanking();		//ランキング並べ替え
		SaveRanking();		//ランキングデータの保存
		g_GameState = DRAW_RANKING;
	}
	if (g_KeyFlg & (PadType ? 16 : 32)) {
		if (selecterY == 0 && selecterX >= 0 && selecterX <= 9) { buf[i++] = (char)48 + selecterX; }
		if (selecterY == 1 && selecterX >= 0 && selecterX <= 12) { buf[i++] = (char)97 + selecterX; }
		if (selecterY == 2 && selecterX >= 0 && selecterX <= 12) { buf[i++] = (char)110 + selecterX; }
		if (selecterY == 3 && selecterX >= 0 && selecterX <= 12) { buf[i++] = (char)65 + selecterX; }
		if (selecterY == 4 && selecterX >= 0 && selecterX <= 12) { buf[i++] = (char)78 + selecterX; }
		if (!isalnum(buf[i - 1])) { i--; }
	}
}

void SortRanking(void)
{
	int i, j;
	RankingData work;

	// 選択法ソート
	for (i = 0; i < RANKING_DATA - 1; i++) {
		for (j = i + 1; j < RANKING_DATA; j++) {
			if (g_Ranking[i].score <= g_Ranking[j].score) {
				work = g_Ranking[i];
				g_Ranking[i] = g_Ranking[j];
				g_Ranking[j] = work;
			}
		}
	}

	// 順位付け
	for (i = 0; i < RANKING_DATA; i++) {
		g_Ranking[i].no = 1;
	}
	// 得点が同じ場合は、同じ順位とする
	// 同順位があった場合の次の順位はデータ個数が加算された順位とする
	for (i = 0; i < RANKING_DATA - 1; i++) {
		for (j = i + 1; j < RANKING_DATA; j++) {
			if (g_Ranking[i].score > g_Ranking[j].score) {
				g_Ranking[j].no++;
			}
		}
	}
}

int  SaveRanking(void)
{
	FILE* fp;
#pragma warning(disable:4996)

	// ファイルオープン
	if ((fp = fopen("dat/rankingdata.txt", "w")) == NULL) {
		/* エラー処理 */
		printf("Ranking Data Error\n");
		return -1;
	}

	// ランキングデータ分配列データを書き込む
	for (int i = 0; i < RANKING_DATA; i++) {
		fprintf(fp, "%2d %10s %10d\n", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
	}

	//ファイルクローズ
	fclose(fp);

	return 0;

}

int ReadRanking(void)
{
	FILE* fp;
#pragma warning(disable:4996)

	//ファイルオープン
	if ((fp = fopen("dat/rankingdata.txt", "r")) == NULL) {
		//エラー処理
		printf("Ranking Data Error\n");
		return -1;
	}

	//ランキングデータ配分列データを読み込む
	for (int i = 0; i < RANKING_DATA; i++) {

		fscanf(fp, "%2d %10s %10d", &g_Ranking[i].no, g_Ranking[i].name, &g_Ranking[i].score);
	}

	//ファイルクローズ
	fclose(fp);

	return 0;
}

void PlayerControl(bool pauseflg) {
	/*if (g_KeyFlg & (PadType ? 2048 : 8192)) {
		if (Pauseflg == false) {
			Pauseflg = true;
		}
		else {
			Pauseflg = false;
		}
	}*/
	static int checkflg = 0;
	static int old_player_angle = 0;
	if (!Pauseflg) {
		if (g_player.flg == TRUE) {
			int i = 0;
			PadSpeedTimer++;
			if (PadSpeedTimer > 10 - i) {
				PadSpeedTimer = 0;
				if (g_player.speed < 6 && g_player.speed > -6) {
					/*++g_player.speed; i += 4;*/ old_player_angle = player_angle; checkflg = 0;
					if (JoyPadX < -300) { g_player.speed++; i += JoyPadX / 200; }
					if (JoyPadX > 300) { g_player.speed++; i += JoyPadX / 200; }
				}
				if (JoyPadX >= -300 && JoyPadX <= 300) {
					i = 0;
					if (g_player.speed > 0)g_player.speed--;
					if (g_player.speed < 0)g_player.speed++;
				}
				if (old_player_angle != player_angle && g_player.speed < 6) {
					if (JoyPadX < -300 && player_angle == -1) {
						g_player.speed--;
					}
				}
			}


			if (old_player_angle != player_angle && g_player.speed > 3) {
				g_player.speed = -2;
			}
			if (player_angle == 1) { g_player.x += g_player.speed; }
			if (player_angle == -1) { g_player.x -= g_player.speed; }
		}

		//	画面をはみ出さないようにする
		if (g_player.x < 32)		g_player.x = 32;

		if (g_player.x > SCREEN_WIDTH - 170)		g_player.x = SCREEN_WIDTH - 170;


		if (g_player.Poisonflg == TRUE && invincibletime++ >= 120) {
			g_player.Poisonflg = false;
			invincibletime = 0;
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		/*if (g_player.Poisonflg == TRUE) { invincibletime++; }*/


	//	プレイヤーの表示
		if (invincibletime % 36 == 0 && g_player.Poisonflg == TRUE) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
		}
		else if (invincibletime % 18 == 0 && g_player.Poisonflg == TRUE) {
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
		if (Pauseflg) {
			if (player_angle == -1)DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, players_img[2], TRUE, FALSE);
			if (player_angle == 1)DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, players_img[3], TRUE, FALSE);
		}
		else {
			if (g_player.flg == TRUE) {
				if (JoyPadX < -300 || player_angle == -1) {
					DrawRotaGraph(g_player.x, g_player.y, 2.3f, -M_PI / 18, players_img[0], TRUE, FALSE); player_angle = -1;
				}
				if (JoyPadX > 300 || player_angle == 1) {
					DrawRotaGraph(g_player.x, g_player.y, 2.3f, -M_PI / 18, players_img[5], TRUE, FALSE); player_angle = 1;
				}
				if (JoyPadX == 0)
				{
					if (player_angle == -1)DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, players_img[1], TRUE, FALSE);
					if (player_angle == 1)DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, players_img[4], TRUE, FALSE);
				}
				if (g_player.speed > 3) {
					if (player_angle == -1)DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, players_img[2], TRUE, FALSE);
					if (player_angle == 1)DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, players_img[3], TRUE, FALSE);
				}
			}
			else {
				DrawRotaGraph(g_player.x, g_player.y, 0.3f, M_PI / 8 * (++g_player.count / 5), players_img[0], TRUE, FALSE);
				if (g_player.count >= 80)		g_player.flg = TRUE;
			}
			//if (invincibletime % 18 == 17 && g_player.Poisonflg == TRUE) {
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		//DrawFormatString(100, 300, 0x000000, "invincibletime:%d", invincibletime);
		DrawBox(500, 0, 640, 480, 0x009900, TRUE);
		SetFontSize(16);
		DrawFormatString(540, 20, 0x000000, "残り時間");
		DrawFormatString(560, 60, 0x000000, "%d", timer / 60);
		DrawFormatString(540, 100, 0x000000, "SCORE");
		DrawFormatString(560, 120, 0x000000, "%d", g_Score);
		DrawFormatString(540, 160, 0x000000, "採った数");
		DrawRotaGraph(550, 220, 1.0f, 0, apple_img[0], TRUE, FALSE);
		DrawRotaGraph(550, 280, 1.0f, 0, apple_img[1], TRUE, FALSE);
		DrawRotaGraph(550, 340, 1.0f, 0, apple_img[2], TRUE, FALSE);
		DrawRotaGraph(550, 400, 1.0f, 0, apple_img[3], TRUE, FALSE);

		DrawFormatString(600, 215, 0xFFFFFF, "%d", apple_count[0]);
		DrawFormatString(600, 275, 0xFFFFFF, "%d", apple_count[1]);
		DrawFormatString(600, 335, 0xFFFFFF, "%d", apple_count[2]);
		DrawFormatString(600, 395, 0xFFFFFF, "%d", apple_count[3]);
}


int HitBoxPlayer(PLAYER* p, Apple* e) {

	//	x,y は中心座標とする
	int sx1 = p->x - (p->w / 2);
	int sy1 = p->y - (p->h / 2);
	int sx2 = sx1 + p->w;
	int sy2 = sy1 + p->h;

	int dx1 = e->GetAppleX() - ((e->GetAppleW() / 2));
	int dy1 = e->GetAppleY() - (e->GetAppleH() / 2);
	int dx2 = dx1 + e->GetAppleW();
	int dy2 = dy1 + e->GetAppleH();

	//	短形が重なっていれば当たり
	if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
		return TRUE;
	}
	return FALSE;
}
int LoadSounds(void)
{
	//タイトルBGM
	if ((TitleBGM = LoadSoundMem("Sound/BGM/ほゎ.wav")) == -1) return -1;
	if ((GameMainBGM = LoadSoundMem("Sound/BGM/ミニマルなマーチ.wav")) == -1) return -1;
	if ((RankingBGM = LoadSoundMem("Sound/BGM/Walking_Ameba.wav")) == -1) return -1;
	if ((EndBGM = LoadSoundMem("Sound/BGM/Small_Happy.wav")) == -1) return -1;
	if ((SE = LoadSoundMem("Sound/SE/select.wav")) == -1) return -1;
}

void DrawPause() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, 640, 480, 0x000000, TRUE);
	DrawFormatStringToHandle(130, 200, 0xffffff, PauseFont, "ぽ　ー　ず");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}