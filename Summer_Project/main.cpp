#include "main.h"
#include <string>	//rankingで使用

#define TIMELIMIT 1800 + 60

LPCSTR font_path = "./Fonts/jkmarugo/JK-Maru-Gothic-M.otf";

enum { DRAW_GAMETITLE, GAME_INIT, DRAW_RANKING, DRAW_HELP, DRAW_END, GAME_MAIN, INPUT_RANKING, END = 99 };
Apple apple[APPLE_MAX];
Apple AppleFunc;
int g_OldKey, g_NowKey, g_KeyFlg;
int MenuFont, PauseFont;
int apple_img[4];
int players_img[9];
int g_GameState = DRAW_GAMETITLE;
int g_Score = 0;
int timer;
int invincibletime;
bool StartFlg = false;
struct PLAYER g_player;
Ranking ranking;

int g_TitleImage;
int g_PosY;
int JoyPadX, JoyPadY;
int player_angle = 1;
int SelectX, SelectY;
int PadTimer, PadSpeedTimer;
int g_WaitTime = 0;
int g_StageImage;
int g_RankingImage;
int g_EndImage;
bool apple_flg;
int apple_x;
int apple_y;
bool Pauseflg;
bool PadType = false;
bool AllReset = false;

int LoadImages();
int LoadSounds();
void DrawPause();
void HelpGuide(int num);
void Sidebar();

int TitleBGM;
int GameMainBGM;
int RankingBGM;
int EndBGM;
int Selecter_SE, OK_SE, Return_SE, Key_Remove_SE;
int GoldenApple_SE, Red_AND_Green_Apple_SE, PoisonApple_SE;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	SetMainWindowText("りんごおとし");
	ChangeWindowMode(TRUE);

	double dNextTime = GetNowCount();

	// フォント読み込み
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// フォント読込エラー処理
		MessageBox(NULL, "フォント読込失敗", "フォント読み込みエラー", MB_OK | MB_ICONERROR);
	}

	ChangeFont("JK丸ゴシック Medium", DX_CHARSET_DEFAULT);

	if (DxLib_Init() == -1)return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	if (LoadImages() == -1)return -1;
	if (LoadSounds() == -1)return -1;
	if (ranking.ReadRanking() == -1)return -1;

	if (GetJoypadNum() == 0) {
		if (MessageBox(NULL, "コントローラーを接続してください。", "コントローラーが未接続です。", MB_OKCANCEL | MB_ICONWARNING) == 1)
			return -1;
	}

	MenuFont = CreateFontToHandle("JK丸ゴシック Medium", 40, 1, DX_CHARSET_DEFAULT);
	PauseFont = CreateFontToHandle("JK丸ゴシック Medium", 80, 1, DX_CHARSET_DEFAULT);

	while (ProcessMessage() == 0 && g_GameState != END) {

		g_OldKey = g_NowKey;
		g_NowKey = GetJoypadInputState(DX_INPUT_PAD1);
		g_KeyFlg = g_NowKey & ~g_OldKey;

		GetJoypadAnalogInput(&JoyPadX, &JoyPadY, DX_INPUT_PAD1);
		if (CheckJoypadXInput(DX_INPUT_PAD1)) { PadType = true; }

		if (PadInput(INPUT_BACK)) { PlaySoundMem(OK_SE, DX_PLAYTYPE_BACK); g_GameState = END; }
		SelectX = 0;
		SelectY = 0;

		if (++PadTimer > 10) {
			PadTimer = 0;
			if (JoyPadX > 100)SelectX = 1;
			if (JoyPadX < -100)SelectX = -1;
			if (JoyPadY > 100)SelectY = 1;
			if (JoyPadY < -100)SelectY = -1;
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

		HelpGuide(g_GameState);


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

	if (AllReset) {
		MenuNo = 0;
		AllReset = false;
	}

	if (CheckSoundMem(GetSound(BGM_Ranking)) == 1)StopSoundMem(GetSound(BGM_Ranking));
	if (CheckSoundMem(GetSound(BGM_Title)) == 0)PlaySoundMem(GetSound(BGM_Title), DX_PLAYTYPE_BACK);

	if (SelectY == 1) { PlaySoundMem(Selecter_SE, DX_PLAYTYPE_BACK); if (++MenuNo > 3)MenuNo = 0; }
	if (SelectY == -1) { PlaySoundMem(Selecter_SE, DX_PLAYTYPE_BACK); if (--MenuNo < 0)MenuNo = 3; }
	if (PadInput(INPUT_A)) {PlaySoundMem(OK_SE, DX_PLAYTYPE_BACK); g_GameState = MenuNo + 1;}
	int change = 0; if (PadInput(INPUT_Y)) { int change = 1;; ChangeVolumeSoundMem(0, GameMainBGM); }
	else if (change == 0) { change = 1; ChangeVolumeSoundMem(255, GameMainBGM); }

	DrawGraph(0, 0, g_TitleImage, FALSE);
	static bool ani = true;
	if (ani == true) {
		static int aniy = rand() % 50;
		/*if(++aniy > 10)*/
		if (SetTimer(0) > 30) DrawFormatStringToHandle(200, 100 + aniy, 0x9c3e26, MenuFont, "り"); aniy = rand() % 20;
		if (GetTimer() > 60) DrawFormatStringToHandle(250, 100 + aniy, 0x9c3e26, MenuFont, "ん"); aniy = rand() % 20;
		if (GetTimer() > 120) DrawFormatStringToHandle(300, 100 + aniy, 0x9c3e26, MenuFont, "ご"); aniy = rand() % 20;
		if (GetTimer() > 180) DrawFormatStringToHandle(350, 100 + aniy, 0x9c3e26, MenuFont, "お"); aniy = rand() % 20;
		if (GetTimer() > 240) DrawFormatStringToHandle(400, 100 + aniy, 0x9c3e26, MenuFont, "と"); aniy = rand() % 20;
		if (GetTimer() > 300) { DrawFormatStringToHandle(450, 100, 0x9c3e26, MenuFont, "し"); ani = false; ResetTimer(); }
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

	DrawRotaGraph(400, 300 + MenuNo * 40, 1.0f, 0, players_img[7], TRUE);
	//DrawRotaGraph(400, 300 + MenuNo * 40, 1.0f, M_PI / 2, SelecterImage, TRUE);
	DrawFormatString(200, 400,0x000000, "%d", JoyPadX);
	DrawFormatString(200, 430, 0x000000, "%d", JoyPadY);
}

void GameInit(void) {
	g_Score = 0;
	StartFlg = true;
	AllReset = true;
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
	g_player.speed = 0;
	g_player.Poisonflg = false;

	g_GameState = GAME_MAIN;
}

void DrawHelp(void) {
	if (PadInput(INPUT_B)) { PlaySoundMem(Return_SE, DX_PLAYTYPE_BACK); g_GameState = DRAW_GAMETITLE; }
	if (PadInput(INPUT_A)) { PlaySoundMem(OK_SE, DX_PLAYTYPE_BACK); g_GameState = GAME_INIT; }

	DrawGraph(0, 0, g_TitleImage, FALSE);
	
	DrawStringToHandle(260, 40, "へるぷ", 0xffffff, MenuFont, 0);

	DrawStringToHandle(20, 120, "このゲームは制限時間：３０秒で", 0xffffff, MenuFont, 0);
	DrawStringToHandle(20, 180, "おおくのりんごをとるゲームです。", 0xffffff, MenuFont, 0);
	DrawStringToHandle(270, 220, "操作方法", 0xffffff, MenuFont, 0);
	DrawOval(180, 290, 18, 10, 0x000000, 1);
	DrawBox(175, 290, 187, 313, 0x000000, 1);
	DrawOval(180, 313, 22, 8, 0x000000, 1);
	DrawStringToHandle(215, 280, "移動", 0x0000ff, MenuFont, 0);
	DrawOval(390, 300, 70, 27, 0x000000, 1);
	DrawStringToHandle(330, 280, "START", 0xffffff, MenuFont);
	DrawStringToHandle(470, 280,"ぽーず", 0xff8c00,MenuFont);
	DrawStringToHandle(20, 330, "A", 0xff0000, MenuFont, 0);
	DrawStringToHandle(44, 330, "ボタンをおして", 0xffffff, MenuFont, 0);
	if (SetTimer(0) < 30) {
		DrawStringToHandle(320, 330, "ゲームスタート", 0xffff00, MenuFont, 0);
	}
	else if(GetTimer() > 60){  ResetTimer(); }

}

void DrawEnd(void) {
	int g_PosY = 0;

	if (CheckSoundMem(TitleBGM) == 1)StopSoundMem(TitleBGM);
	if (CheckSoundMem(EndBGM) == 0)PlaySoundMem(EndBGM, DX_PLAYTYPE_BACK);

	//エンド画像表示
	DrawExtendGraph(0, 0,640,480, g_EndImage, FALSE);
	//エンディング表示
	if (SetTimer(0) < 600) { g_PosY = 300 - GetTimer() / 2; }

	SetFontSize(24);
	DrawString(140, 80 + g_PosY, "タイトル　　　りんごのもり", 0xFFFFFF, 0);
	DrawString(140, 110 + g_PosY, "バージョン　　1.0", 0xFFFFFF, 0);
	DrawString(140, 140 + g_PosY, "最終更新日　　2022年6月28日", 0xFFFFFF, 0);
	DrawString(140, 170 + g_PosY, "制作者　　　　わん,ゆうた", 0xFFFFFF, 0);
	DrawString(140, 200 + g_PosY, "　　　　　　　しょうご,しき", 0xFFFFFF, 0);
	DrawString(140, 240 + g_PosY, "素材", 0xFFFFFF);
	DrawString(140, 270 + g_PosY, "　画像　　　　いらすと屋", 0xFFFFFF);
	DrawString(140, 300 + g_PosY, "　BGM　　　　 DOVA-SYNDROME", 0xFFFFFF, 0);
	DrawString(140, 325 + g_PosY, "　SE　　　　　効果音工房", 0xFFFFFF, 0);

	//タイムの加算処理＆終了
	if (SetTimer(0) > 900) { ResetTimer(); g_GameState = END; }

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
	Sidebar();

	if (PadInput(INPUT_START)) {
		if (Pauseflg == false) {
			PlaySoundMem(OK_SE, DX_PLAYTYPE_BACK);
			Pauseflg = true;
		}
		else {
			PlaySoundMem(Return_SE, DX_PLAYTYPE_BACK);
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
	if ((g_TitleImage = LoadGraph("images/Title.png")) == -1)return-1;
	if ((g_EndImage = LoadGraph("images/End.png")) == -1)return-1;
	if (LoadDivGraph("images/apple.png", 4, 4, 1, 50, 50, apple_img) == -1)return -1;	//リンゴ
	if ((g_StageImage = LoadGraph("images/background.png")) == -1)return-1;
	if ((g_RankingImage = LoadGraph("images/Ranking.png")) == -1)return-1;
	if (LoadDivGraph("images/player.png", 9, 3, 3, 32, 32, players_img) == -1)return -1;
	return 0;
}

void DrawRanking(void) {

	if (CheckSoundMem(TitleBGM) == 1)StopSoundMem(TitleBGM);
	if (CheckSoundMem(GameMainBGM) == 1)StopSoundMem(GameMainBGM);
	if (CheckSoundMem(RankingBGM) == 0)PlaySoundMem(RankingBGM, DX_PLAYTYPE_BACK);

	if (PadInput(INPUT_B)) { PlaySoundMem(Return_SE, DX_PLAYTYPE_BACK); g_GameState = DRAW_GAMETITLE; }

	DrawGraph(0, 0, g_RankingImage, FALSE);

	for (int i = 0; i < RANKING_DATA; i++) {
		DrawFormatStringToHandle(50, 120 + i * 50, 0xffffff, MenuFont, "%2d  %-10s", g_Ranking[i].no, g_Ranking[i].name);
		DrawFormatStringToHandle(380, 120 + i * 50, 0xffffff, MenuFont, "%5d", g_Ranking[i].score);

		if (SetTimer(0) < 120) {
			DrawString(150, 390, "---- Bボタンーをおしてタイトルへもどる ----", 0xffffff, 0);
		}
		else if (GetTimer() > 360) { ResetTimer(); }
	}
}

void InputRanking(void)
{
	if (CheckSoundMem(GameMainBGM) == 1)StopSoundMem(GameMainBGM);
	if (CheckSoundMem(RankingBGM) == 0)PlaySoundMem(RankingBGM, DX_PLAYTYPE_BACK);
	//ランキング画像表示
	DrawGraph(0, 0, g_RankingImage, FALSE);

	static char default_char[10] = "_________";
	static char buf[10] = "_________";
	static int input_i = 0;
	static int errorflg = 0;

	// 名前入力指示文字列の描画
	DrawFormatStringToHandle(120, 100, 0xFFFFFF, MenuFont, "ランキングに登録します");
	DrawFormatStringToHandle(65, 150, 0xFFFFFF, MenuFont, "名前を英字で入力してください");
	if (GetTimer() < 30) {
		DrawString(180, 420, "---- STARTボタンをおして名前決定！ ----", 0xffffff, 0);
	}
	else if (GetTimer() > 60) { ResetTimer(); }
	// 名前の入力
	DrawFormatStringToHandle(120, 200, 0xFFFFFF, MenuFont, "> ");
	DrawBox(190, 200, 425, 250, 0x000055, TRUE);

	if (errorflg == 1 && (60 > timer++)) {
		DrawFormatStringToHandle(120, 220, 0xff1493,MenuFont, "名前を入力してください");
	}
	else if (timer > 60) { timer = 0; errorflg = 0; }

		for (int i = 0; input_i > i; i++) { DrawFormatStringToHandle(195 + i * 25, 205, 0xFFFFFF, MenuFont, "%c", buf[i]); }
		/*if(display > input_i){ DrawFormatStringToHandle(200 + input_i * 25, 205, 0xFFFFFF, MenuFont, "%c", buf[input_i - 1]); }*/

	if (SetTimer(0) < 30) {
		//DrawFormatStringToHandle(200, 205, 0xFFFFFF, MenuFont, "%s", buf);
		for (int display = 10; input_i < display; display--) {
			DrawFormatStringToHandle(170 + display * 25, 205, 0xFFFFFF, MenuFont, "%c", buf[display - 1]);
		}
	}
	else if (GetTimer() > 60) { ResetTimer(); }

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
	if (SelectX == 1)if (++selecterX > 12)selecterX = 0; else if (selecterY == 0 && selecterX > 9)selecterX = 0;
	if (SelectX == -1)if (--selecterX < 0)selecterX = 12; if (selecterY == 0 && selecterX > 9)selecterX = 9;
	if (SelectY == 1)if (++selecterY > 4)selecterY = 0;
	if (SelectY == -1)if (--selecterY < 0)selecterY = 4;
	if (PadInput(INPUT_A)) {}
	if (SelectX == -1 || SelectX == 1 || SelectY == -1 || SelectY == 1) { PlaySoundMem(Selecter_SE, DX_PLAYTYPE_BACK); }
	DrawBox(120 + 30 * selecterX, 250 + 30 * selecterY, 150 + 30 * selecterX, 280 + 30 * selecterY, 0x696969, FALSE);


	if (PadInput(INPUT_START)) {
		PlaySoundMem(OK_SE, DX_PLAYTYPE_BACK);
		if (input_i <= 0) { errorflg = 1; }
		else {
			buf[input_i] = '\0';
			std::string buf_str = buf;
			buf_str = buf_str.erase(input_i);
			strcpyDx(ranking.GetRankingName(RANKING_DATA - 1), buf_str.c_str());
			//strcpyDx(g_Ranking[RANKING_DATA - 1].name, buf_str.c_str());

			g_Ranking[RANKING_DATA - 1].score = g_Score;	// ランキングデータの１０番目にスコアを登録
			ranking.SortRanking();		//ランキング並べ替え
			ranking.SaveRanking();		//ランキングデータの保存
			input_i = 0;
			strcpyDx(buf, default_char);
			g_GameState = DRAW_RANKING;
		}
	}
	if (PadInput(INPUT_A) && input_i < 9) {
		PlaySoundMem(OK_SE, DX_PLAYTYPE_BACK);
		if (selecterY == 0 && selecterX >= 0 && selecterX <= 9) { buf[input_i++] = (char)48 + selecterX; }
		if (selecterY == 1 && selecterX >= 0 && selecterX <= 12) { buf[input_i++] = (char)97 + selecterX; }
		if (selecterY == 2 && selecterX >= 0 && selecterX <= 12) { buf[input_i++] = (char)110 + selecterX; }
		if (selecterY == 3 && selecterX >= 0 && selecterX <= 12) { buf[input_i++] = (char)65 + selecterX; }
		if (selecterY == 4 && selecterX >= 0 && selecterX <= 12) { buf[input_i++] = (char)78 + selecterX; }
		if (!isalnum(buf[input_i - 1])) { buf[--input_i] = '_'; }
	}
	if (PadInput(INPUT_B)) {
		PlaySoundMem(Key_Remove_SE, DX_PLAYTYPE_BACK);
		if(input_i > 0)buf[--input_i] = '_'; 
	}
}


void PlayerControl(bool pauseflg) {
	static int checkflg = 0;
	static int old_player_angle = 0;
	if (!Pauseflg) {
		if (g_player.flg == TRUE) {
			int i = 0;
			PadSpeedTimer++;
			if (PadSpeedTimer > 20 - i) {
				PadSpeedTimer = 0;
				if (g_player.speed < 6 && g_player.speed > -6) {
					/*++g_player.speed; i += 4;*/ old_player_angle = player_angle; checkflg = 0;
					if (JoyPadX < -100) { g_player.speed++; /*i += JoyPadX / 200;*/ }
					if (JoyPadX > 100) { g_player.speed++; /*i += JoyPadSX / 200;*/ }
				}
				if (JoyPadX >= -100 && JoyPadX <= 100) {
					i = 0;
					if (g_player.speed > 0)g_player.speed-= 2;
					if (g_player.speed < 0)g_player.speed++;
				}
				if (old_player_angle != player_angle && g_player.speed < 6) {
					if (JoyPadX < -100 && player_angle == -1) {
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
		if (g_player.x < 32) { g_player.x = 32; g_player.speed = 0; }

		if (g_player.x > SCREEN_WIDTH - 170) { g_player.x = SCREEN_WIDTH - 170; g_player.speed = 0;}


		if (g_player.Poisonflg == TRUE && invincibletime++ >= 120) {
			g_player.Poisonflg = false;
			invincibletime = 0;
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

	}
		if (Pauseflg) {
			if (player_angle == -1)DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, players_img[2], TRUE, FALSE);
			if (player_angle == 1)DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, players_img[3], TRUE, FALSE);
		}
		else if((g_player.Poisonflg == TRUE && g_WaitTime++ < 20) || g_player.Poisonflg == FALSE){
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
		else if (g_WaitTime > 40) {
			g_WaitTime = 0;
		}
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
	//BGM
	if ((TitleBGM = LoadSoundMem("Sound/BGM/Title.wav")) == -1) return -1;
	if ((GameMainBGM = LoadSoundMem("Sound/BGM/GameMain.wav")) == -1) return -1;
	if ((RankingBGM = LoadSoundMem("Sound/BGM/Ranking.wav")) == -1) return -1;
	if ((EndBGM = LoadSoundMem("Sound/BGM/End.wav")) == -1) return -1;
	//SE
	if ((Selecter_SE = LoadSoundMem("Sound/SE/Selecter.wav")) == -1) return -1;
	if ((OK_SE = LoadSoundMem("Sound/SE/OK.wav")) == -1) return -1;
	if ((Return_SE = LoadSoundMem("Sound/SE/Return.wav")) == -1) return -1;
	if ((Key_Remove_SE = LoadSoundMem("Sound/SE/Key_Remove.wav")) == -1) return -1;
	if ((GoldenApple_SE = LoadSoundMem("Sound/SE/gold_apple.wav")) == -1) return -1;
	if ((PoisonApple_SE = LoadSoundMem("Sound/SE/Poison_Apple.wav")) == -1) return -1;
	if ((Red_AND_Green_Apple_SE = LoadSoundMem("Sound/SE/Red&Green_Apple.wav")) == -1) return -1;
	return 0;
}

void DrawPause() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, 640, 480, 0x000000, TRUE);
	DrawFormatStringToHandle(130, 200, 0xffffff, PauseFont, "ぽ　ー　ず");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void HelpGuide(int num) {
	switch (num)
	{
	case DRAW_GAMETITLE:
		DrawOval(180, 450, 10, 5, 0x000000, 1);
		DrawBox(177, 450, 185, 463, 0x000000, 1);
		DrawOval(180, 463, 12, 5, 0x000000, 1);
		DrawFormatString(200, 450, 0xFFFFFF, "移動");
		DrawCircle(304, 457, 10, 0x000000, 1);
		DrawFormatString(300, 450, 0xFFFFFF, "A  決定");
		DrawOval(420, 457, 28, 10, 0x000000, 1);
		DrawFormatString(400, 450, 0xFFFFFF, "BACK   終了");
		break;

	case DRAW_RANKING:
		DrawCircle(304, 457, 10, 0x000000, 1);
		DrawFormatString(300, 450, 0xFFFFFF, "B  戻る");
		break;

	case DRAW_HELP:
		DrawCircle(204, 457, 10, 0x000000, 1);
		DrawFormatString(200, 450, 0xFFFFFF, "B  戻る");
		DrawCircle(354, 457, 10, 0x000000, 1);
		DrawFormatString(350, 450, 0xFFFFFF, "A  ゲームスタート");
		break;

	case GAME_MAIN:
		int color,color2;
		if (Pauseflg == true) {
			color = 0xFFFFFF; color2 = 0xff8c00;
		}
		else { color = 0x000000; color2 = 0xFFFFFF; SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);}
		DrawOval(180, 450, 10, 5, color, 1);
		DrawBox(177, 450, 185, 463, color, 1);
		DrawOval(180, 463, 12, 5, color, 1);
		DrawFormatString(200, 450, color2, "移動");
		DrawOval(284, 457, 30, 10, color, 1);
		DrawFormatString(260, 450, color2, "START ポーズ");
		DrawOval(404, 457, 28, 10, color, 1);
		DrawFormatString(384, 450, color2, "BACK   終了");
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;

	case INPUT_RANKING:
		DrawCircle(164, 457, 10, 0x000000, 1);
		DrawFormatString(160, 450, 0xFFFFFF, "B  1文字削除");
		DrawCircle(304, 457, 10, 0x000000, 1);
		DrawFormatString(300, 450, 0xFFFFFF, "A  入力");
		DrawOval(424, 457, 30, 10, 0x000000, 1);
		DrawFormatString(400, 450, 0xFFFFFF, "START 名前確定");
		break;

	default:
		break;
	}
}

void Sidebar() {
	DrawBox(500, 0, 640, 480, 0x009900, TRUE);
	DrawFormatString(540, 20, 0xFFFFFF, "残り時間");
	DrawFormatStringToHandle(545, 50, 0xffff00,MenuFont, "%2d", timer / 60);
	if(timer / 60 <= 10){ DrawFormatStringToHandle(545, 50, 0xff4500, MenuFont, "%2d", timer / 60); }
	DrawFormatString(545, 100, 0xffff99, "SCORE");
	DrawFormatString(550, 120, 0xffff99, "%4d", g_Score);
	DrawFormatString(540, 160, 0xFFFFFF, "採った数");
	DrawRotaGraph(550, 220, 1.0f, 0, apple_img[0], TRUE, FALSE);
	DrawRotaGraph(550, 280, 1.0f, 0, apple_img[1], TRUE, FALSE);
	DrawRotaGraph(550, 340, 1.0f, 0, apple_img[2], TRUE, FALSE);
	DrawRotaGraph(550, 400, 1.0f, 0, apple_img[3], TRUE, FALSE);

	DrawFormatString(600, 215, 0xFFFFFF, "%d", apple_count[0]);
	DrawFormatString(600, 275, 0xFFFFFF, "%d", apple_count[1]);
	DrawFormatString(600, 335, 0xFFFFFF, "%d", apple_count[2]);
	DrawFormatString(600, 395, 0xFFFFFF, "%d", apple_count[3]);
}

int SetTimer(int num) {
	if (num == 0) {
		++g_WaitTime;
		return g_WaitTime;
	}
	else {
		g_WaitTime = num;
	}
	return g_WaitTime;
}

int GetTimer(void) {
	return g_WaitTime;
}

void ResetTimer(void) {
	g_WaitTime = 0;
}

bool PadInput(int Key) {
	if (Key > 0 && Key < 9) {
		if (Key == INPUT_A) {
			if (g_KeyFlg & (PadType ? XINPUT_A : DINPUT_A))return true;
		}
		if (Key == INPUT_B) {
			if (g_KeyFlg & (PadType ? XINPUT_B : DINPUT_B))return true;
		}
		if (Key == INPUT_X) {
			if (g_KeyFlg & (PadType ? XINPUT_X : DINPUT_X))return true;
		}
		if (Key == INPUT_Y) {
			if (g_KeyFlg & (PadType ? XINPUT_Y : DINPUT_Y))return true;
		}
		if (Key == INPUT_BACK) {
			if (g_KeyFlg & (PadType ? XINPUT_BACK : DINPUT_BACK))return true;
		}
		if (Key == INPUT_START) {
			if (g_KeyFlg & (PadType ? XINPUT_START : DINPUT_START))return true;
		}
	}
	return false;
}

int GetSound(int sound_name) {
	if (sound_name == BGM_Title)return TitleBGM;
	if (sound_name == BGM_GameMain)return GameMainBGM;
	if (sound_name == BGM_Ranking)return RankingBGM;
	if (sound_name == BGM_END)return EndBGM;

	if (sound_name == SE_Selecter)return Selecter_SE;
	if (sound_name == SE_OK)return OK_SE;
	if (sound_name == SE_Return)return Return_SE;
	if (sound_name == SE_Key_Remove)return Key_Remove_SE;
	if (sound_name == SE_GoldenApple)return GoldenApple_SE;
	if (sound_name == SE_Red_AND_Green_Apple)return Red_AND_Green_Apple_SE;
	if (sound_name == SE_PoisonApple)return PoisonApple_SE;
	return 0;
}

int GetImage(int imagename){
	if (imagename == Image_Title)return g_TitleImage;
	if (imagename == Image_Stage)return g_StageImage;
	if (imagename == Image_Ranking)return g_RankingImage;
	if (imagename == Image_End)return g_EndImage;
	return 0;
}