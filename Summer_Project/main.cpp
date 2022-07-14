#include "main.h"

#define TIMELIMIT 1800 + 60


int g_OldKey, g_NowKey, g_KeyFlg;

int g_GameState = DRAW_GAMETITLE;
int g_Score = 0;
int TimeLimit;	//制限時間用
bool StartFlg = false;
Ranking ranking;
PLAYER player;
Apple apple[APPLE_MAX];

int JoyPadX, JoyPadY,PadTimer;	//アナログスティック傾きXY
int SelectX, SelectY;			//カーソル移動用XY(アナログスティック)
int g_WaitTime = 0;				//タイマー関数用変数
bool Pauseflg;					//ポーズ用フラグ
bool PadType = false;			//XInputの有効性を検出
bool AllReset = false;			//タイトル画面とランキング画面の選択位置をリセットする。(2回目対処用)

int LoadImages();				//画像データ読み込み
int LoadSounds();				//音声データ読み込み
void DrawPause();				//ポーズ用
void HelpGuide(int num);		//操作説明
void Sidebar();					//制限時間、スコア、りんごの取得数表示用

/********************素材*********************/
//フォント
LPCSTR font_path = "./Fonts/jkmarugo/JK-Maru-Gothic-M.otf";
int MenuFont, PauseFont;	//フォントハンドル
//画像
int g_TitleImage, g_StageImage, g_RankingImage, g_EndImage;		//画面表示用
int players_img[9];			//プレイヤー操作画像[左(3),右(3),前(3)]
int apple_img[4];			//りんごの画像[赤りんご、緑りんご、金のりんご、毒りんご]
//音声
int TitleBGM, GameMainBGM, RankingBGM, EndBGM;					//BGM
int Selecter_SE, OK_SE, Return_SE, Key_Remove_SE;				//操作音SE
int GoldenApple_SE, Red_AND_Green_Apple_SE, PoisonApple_SE;		//りんご用SE



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
			ranking.DrawRanking();
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
			ranking.InputRanking();
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

	if (GetAllReset()) {
		MenuNo = 0;
		SetAllReset(false);
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
	TimeLimit = TIMELIMIT;
	ResetTimer();
	Pauseflg = false;

	apple->AppleInit();
	player.ResetPlayer();

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
	static int g_PosY = 0;

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
	apple->AppleControl();
	player.PlayerControl();
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

		if (TimeLimit-- == 0) {
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
	DrawFormatStringToHandle(545, 50, 0xffff00,MenuFont, "%2d", TimeLimit / 60);
	if(TimeLimit / 60 <= 10){ DrawFormatStringToHandle(545, 50, 0xff4500, MenuFont, "%2d", TimeLimit / 60); }
	DrawFormatString(545, 100, 0xffff99, "SCORE");
	DrawFormatString(550, 120, 0xffff99, "%4d", g_Score);
	DrawFormatString(540, 160, 0xFFFFFF, "採った数");
	DrawRotaGraph(550, 220, 1.0f, 0, apple_img[0], TRUE, FALSE);
	DrawRotaGraph(550, 280, 1.0f, 0, apple_img[1], TRUE, FALSE);
	DrawRotaGraph(550, 340, 1.0f, 0, apple_img[2], TRUE, FALSE);
	DrawRotaGraph(550, 400, 1.0f, 0, apple_img[3], TRUE, FALSE);

	DrawFormatString(600, 215, 0xFFFFFF, "%d", apple->GetAppleCount(RED_APPLE));
	DrawFormatString(600, 275, 0xFFFFFF, "%d", apple->GetAppleCount(GREEN_APPLE));
	DrawFormatString(600, 335, 0xFFFFFF, "%d", apple->GetAppleCount(GOLD_APPLE));
	DrawFormatString(600, 395, 0xFFFFFF, "%d", apple->GetAppleCount(BLACK_APPLE));
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

int GetAnalogInput(int xy) {
	if (-1000 <= xy && xy <= 1000) {
		if (xy == AnalogInput_X)return JoyPadX;
		if (xy == AnalogInput_Y)return JoyPadY;
	}
	return 0;
}

bool PadInput(int Key) {
	if (Key > 0 && Key < 9) {
		if (Key == INPUT_A) {
			if (g_KeyFlg & (PadType ? 16 : 32))return true;
		}
		if (Key == INPUT_B) {
			if (g_KeyFlg & (PadType ? 32 : 64))return true;
		}
		if (Key == INPUT_X) {
			if (g_KeyFlg & (PadType ? 64 : 16))return true;
		}
		if (Key == INPUT_Y) {
			if (g_KeyFlg & (PadType ? 128 : 128))return true;
		}
		if (Key == INPUT_BACK) {
			if (g_KeyFlg & (PadType ? 1024 : 4096))return true;
		}
		if (Key == INPUT_START) {
			if (g_KeyFlg & (PadType ? 2048 : 8192))return true;
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

int GetPlayerImage(int player_status) {
	if (player_status == Image_LeftPlayer)return players_img[0];
	if (player_status == Image_IDOL_LeftPlayer)return players_img[1];
	if (player_status == Image_DASH_LeftPlayer)return players_img[2];
	if (player_status == Image_DASH_RightPlayer)return players_img[3];
	if (player_status == Image_IDOL_RightPlayer)return players_img[4];
	if (player_status == Image_RightPlayer)return players_img[5];
	if (player_status == Image_TOP_Player)return players_img[6];
	if (player_status == Image_TOP_IDOLPlayer)return players_img[7];
	if (player_status == Image_TOP_Player2)return players_img[8];
	return 0;
}

int GetAppleImage(int type) {
	if (type == Image_RedApple)return apple_img[0];
	if (type == Image_GreenApple)return apple_img[1];
	if (type == Image_GoldenApple)return apple_img[2];
	if (type == Image_PoisonApple)return apple_img[3];
	return 0;
}

int GetFont(int num) {
	if (num == 1)return MenuFont;
	if (num == 2)return PauseFont;
	return 0;
}

int GetSelect(int xy) {
	if (xy == Select_X)return SelectX;
	if (xy == Select_Y)return SelectY;
	return 0;
}

bool isPause(void) {
	return Pauseflg;
}

int GetGameStatus(void) {
	if (0 <= g_GameState && g_GameState <= 6)return g_GameState;
	return -1;
}

void SetGameStatus(int GameStatus) {
	if (0 <= GameStatus && GameStatus <= 6)g_GameState = GameStatus;
}

int GetTimeLimit(void) {
	if (0 <= TimeLimit && TimeLimit <= 99999)return TimeLimit;
	return -1;
}

int GetMainScore(void) {
	if (-99999 <= g_Score && g_Score <= 99999)return g_Score;
	return -1;
}

void SetMainScore(int score) {
	if (-99999 <= score && score <= 99999)g_Score = score;
}

bool GetStartFlg(void) {
	if (StartFlg == (true || false))return StartFlg;
		return false;
}
void SetStartFlg(bool Flg) {
	if (Flg == true) StartFlg = true;
	if (Flg == false) StartFlg = false;
}

bool GetAllReset(void) {
	if (StartFlg == (true || false))return StartFlg;
	return false;
}

void SetAllReset(bool Flg) {
	if (Flg == true) StartFlg = true;
	if (Flg == false) StartFlg = false;
}