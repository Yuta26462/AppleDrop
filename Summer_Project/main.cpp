#include "main.h"

#define TIMELIMIT 1800 + 60

LPCSTR font_path = "../Fonts/jkmarugo/JK-Maru-Gothic-M.otf";

Apple apple[APPLE_MAX];
Apple AppleFunc;
int g_OldKey, g_NowKey, g_KeyFlg;
int MenuFont;
int apple_img[4];
int players_img[6];
int g_GameState = 0;
int g_Score = 0;
int timer;
int g_RankingImage;
bool StartFlg = false;
struct PLAYER g_player;
struct RankingData g_Ranking[RANKING_DATA];

int g_TitleImage;
int g_Cone;
int g_PosY;
int JoyPadX, JoyPadY;
int SelectY;
int PadTimer;
int g_WaitTime = 0;
int g_EndImage;
int g_StageImage;
bool apple_flg;
int apple_x;
int apple_y;
bool Pflg;

int LoadImages();
int LoadSounds();

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

	while (ProcessMessage() == 0 && g_GameState != 99 && !(g_KeyFlg & PAD_INPUT_START)) {

		g_OldKey = g_NowKey;
		g_NowKey = GetJoypadInputState(DX_INPUT_PAD1);
		g_KeyFlg = g_NowKey & ~g_OldKey;

		GetJoypadAnalogInput(&JoyPadX,&JoyPadY,DX_INPUT_PAD1);
		SelectY = 0;
		
		//if (PadTimer > 300) {
		//	++PadTimer;
			if (JoyPadY > 800)SelectY = 1;
			if (JoyPadY < -800)SelectY = -1;
		//}
		//else { PadTimer = 0; }
		SetJoypadDeadZone(DX_INPUT_PAD1,0.8f);
		

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

	if (g_KeyFlg & PAD_INPUT_DOWN)
	{
		PlaySoundMem(SE, DX_PLAYTYPE_BACK);
		if (++MenuNo > 3)MenuNo = 0;
	}
	if (g_KeyFlg & PAD_INPUT_UP)
	{
		PlaySoundMem(SE, DX_PLAYTYPE_BACK);
		if (--MenuNo < 0)MenuNo = 3;
	}

	//	キーボード操作用
	/*if (g_KeyFlg & PAD_INPUT_DOWN)if (++MenuNo > 3)MenuNo = 0;
	if (g_KeyFlg & PAD_INPUT_UP)if (--MenuNo < 0)MenuNo = 3;*/
	if (SelectY == 1)if (++MenuNo > 3)MenuNo = 0;
	if (SelectY == -1)if (--MenuNo < 0)MenuNo = 3;
	if (SelectY == 1 || SelectY == -1)StartJoypadVibration(DX_INPUT_PAD1, 100, 2); if (++g_WaitTime < 60)StopJoypadVibration(DX_INPUT_PAD1); g_WaitTime = 0;
	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_1)g_GameState = MenuNo + 1;

	DrawGraph(0, 0, g_TitleImage, FALSE);
	
	
	
	DrawFormatStringToHandle(420, 280, 0x9c3e26, MenuFont ,"すたーと" );
	DrawFormatStringToHandle(420, 320, 0x9c3e26, MenuFont , "らんきんぐ");
	DrawFormatStringToHandle(420, 360, 0x9c3e26, MenuFont ,"へるぷ");
	DrawFormatStringToHandle(420, 400, 0x9c3e26, MenuFont , "えんど");

	DrawRotaGraph(400, 300 + MenuNo * 40, 0.7f, M_PI / 2, g_Cone, TRUE);

	char string[64];
	wsprintf(string, "X = %d", JoyPadX);
	DrawString(200, 0, string, 0x000000);

	wsprintf(string, "Y = %d", JoyPadY);
	DrawString(200, 100, string, 0x000000);

}

void GameInit(void) {
	g_Score = 0;
	StartFlg = true;
	timer = TIMELIMIT;
	Pflg = false;
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

	g_GameState = 5;
}

void DrawRanking(void) {

	if (CheckSoundMem(TitleBGM) == 1)StopSoundMem(TitleBGM);
	if (CheckSoundMem(RankingBGM) == 0)PlaySoundMem(RankingBGM, DX_PLAYTYPE_BACK);

	//	スペースキーでメニューに戻る
	if (g_KeyFlg & PAD_INPUT_2)g_GameState = 0;

	DrawGraph(0, 0, g_RankingImage, FALSE);


	SetFontSize(18);
	for (int i = 0; i < RANKING_DATA; i++) {
		DrawFormatString(50, 170 + i * 25, 0xffffff, "%2d  %-10s  %10d", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
		DrawString(100, 450, "---- スペースキーを押してタイトルへ戻る ----", 0xffffff, 0);
	}
}

void DrawHelp(void) {
	if (g_KeyFlg & PAD_INPUT_2)	g_GameState = 0;

	DrawGraph(0, 0, g_TitleImage, FALSE);
	SetFontSize(16);
	DrawString(20, 120, "ヘルプ画面", 0xffffff, 0);

	DrawString(20, 160, "これは障害物を避けながら", 0xffffff, 0);
	DrawString(20, 180, "走り続けるゲームです", 0xffffff, 0);
	DrawString(20, 450, "---- スペースキーを押してタイトルへ戻る ----", 0xffffff, 0);
}

void DrawEnd(void) {
	int g_PosY = 0;

	if (CheckSoundMem(TitleBGM) == 1)StopSoundMem(TitleBGM);
	if (CheckSoundMem(EndBGM) == 0)PlaySoundMem(EndBGM, DX_PLAYTYPE_BACK);

	//エンド画像表示
	DrawGraph(0, 0, g_EndImage, FALSE);
	//エンディング表示
	if (++g_WaitTime < 600) g_PosY = 300 - g_WaitTime / 2;

	SetFontSize(24);
	DrawString(100, 170 + g_PosY, "タイトル　　　りんごのもり", 0xFFFFFF,0);
	DrawString(100, 200 + g_PosY, "バージョン　　1.0", 0xFFFFFF,0);
	DrawString(100, 230 + g_PosY, "最終更新日　　2022年6月28日", 0xFFFFFF,0);
	DrawString(100, 260 + g_PosY, "制作者　　　　わん,ゆうた", 0xFFFFFF,0);
	DrawString(100, 290 + g_PosY, "　　　　　　　しょうご,しき", 0xFFFFFF,0);
	DrawString(100, 310 + g_PosY, "素材利用", 0xFFFFFF);
	DrawString(100, 340 + g_PosY, "　BGM　　,　 DOVA-SYNDROME", 0xFFFFFF,0);
	DrawString(100, 365 + g_PosY, "　SE　　　　　効果音工房", 0xFFFFFF,0);

	//タイムの加算処理＆終了
	if (++g_WaitTime > 900) g_GameState = 99;

	DeleteFontToHandle(MenuFont);
}

void GameMain(void) {

	if (CheckSoundMem(TitleBGM) == 1)StopSoundMem(TitleBGM);
	if (CheckSoundMem(TitleBGM) == 0)PlaySoundMem(GameMainBGM, DX_PLAYTYPE_BACK);
	
	if (Pflg == false) {
		if (timer-- == 0) {
			if (g_Ranking[RANKING_DATA - 1].score >= g_Score) {
				g_GameState = 0;
			}
			else {
				g_GameState = 6;
			}
		}
	}
	if (CheckSoundMem(GameMainBGM) == 0)PlaySoundMem(GameMainBGM, DX_PLAYTYPE_BACK);

	if (timer-- == 0) {
		g_GameState = 6;
	}

	DrawGraph(0, 0, g_StageImage, FALSE);
	DrawFormatString(280, 250, 0x000000, "%d", Pflg);
	if (Pflg == true) {
		DrawString(300, 250, "PAUSE", 0x000000);
	}

	AppleFunc.AppleControl(Pflg);

	PlayerControl();

	//DrawFormatStringToHandle(270, 25, 0x000000, MenuFont, "x:%d  y:%d", MouseX, MouseY);	//デバック用 座標確認
}

void DrawGameOver(void) {


	//BackScrool();

	//spflag = 1;

	/*g_Score = (g_Mileage / 10 * 10) + g_EnemyCount4 * 300 + g_EnemyCount3 * 50 + g_EnemyCount2 * 100 + g_EnemyCount1 * 200;*/

	if (g_KeyFlg & PAD_INPUT_M) {
		if (g_Ranking[RANKING_DATA - 1].score >= g_Score) {
			g_GameState = 0;
		}
		else {
			g_GameState = 7;
		}
	}

	DrawGraph(0, 0, g_StageImage, FALSE);
	AppleFunc.AppleControl(Pflg);

	DrawFormatString(300, 200, 0x000000, "Pflg:%d", Pflg);
	if (Pflg == true) {
		DrawString(320, 200, "POUSE", 0x000000);
	}

	PlayerControl();

	//DrawFormatStringToHandle(270, 25, 0x000000, MenuFont, "x:%d  y:%d", MouseX, MouseY);	//デバック用 座標確認
}



int LoadImages() {
	if (LoadDivGraph("images/apple.png", 5, 4, 1, 50, 50, apple_img) == -1) return -1;

	if ((g_TitleImage = LoadGraph("images/Title.png")) == -1)return-1;
	if ((g_Cone = LoadGraph("images/cone.bmp")) == -1)return-1;
	//if ((g_Item[0] = LoadGraph("images/greenapple.png")) == -1)return-1;
	//if ((g_Item[1] = LoadGraph("images/apple.png")) == -1)return-1;
	if ((g_EndImage = LoadGraph("images/background.png")) == -1)return-1;
	if (LoadDivGraph("images/apple.png", 4, 4, 1, 50, 50, apple_img) == -1)return -1;	//リンゴ
	if ((g_StageImage = LoadGraph("images/pause.png")) == -1)return-1;
	if (LoadDivGraph("images/player.png", 6, 3, 2, 32, 32, players_img) == -1)return -1;	//リンゴ
	return 0;
}

void InputRanking(void)
{
	//ランキング画像表示
	DrawGraph(0, 0, g_RankingImage, FALSE);

	// フォントサイズの設定
	SetFontSize(20);

	// 名前入力指示文字列の描画
	DrawString(150, 240, "ランキングに登録します", 0xFFFFFF);
	DrawString(150, 270, "名前を英字で入力してください", 0xFFFFFF);

	// 名前の入力
	DrawString(150, 310, "> ", 0xFFFFFF);
	DrawBox(160, 305, 300, 335, 0x000055, TRUE);
	if (KeyInputSingleCharString(170, 310, 10, g_Ranking[RANKING_DATA - 1].name, FALSE) == 1) {
		g_Ranking[RANKING_DATA - 1].score = g_Score;	// ランキングデータの１０番目にスコアを登録
		SortRanking();		//ランキング並べ替え
		SaveRanking();		//ランキングデータの保存
		g_GameState = 2;	//ゲームモードの変更
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
	if ((fp = fopen("dat/05/rankingdata.txt", "w")) == NULL) {
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
	if ((fp = fopen("dat/05/rankingdata.txt", "r")) == NULL) {
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

void PlayerControl() {

	if (g_KeyFlg & PAD_INPUT_M) {
		if (Pflg == false) {
			Pflg = true;
		}
		else {
			Pflg = false;
		}
	}

	//	上下左右移動
	if (g_player.flg == TRUE && Pflg == false) {
		
		if (JoyPadX < -300 || JoyPadX > 300)PadTimer++;
		if (PadTimer < 30) {
			if (g_player.speed < 6) { g_player.speed++; PadTimer = 0; }
		}
		if (JoyPadX < -300)	g_player.x -= g_player.speed;
		if (JoyPadX > 300)	g_player.x += g_player.speed;
	}DrawFormatString(100, 230, 0x000000, "%d", PadTimer);

	//	画面をはみ出さないようにする
	if (g_player.x < 32)		g_player.x = 32;

	if (g_player.x > SCREEN_WIDTH - 160)		g_player.x = SCREEN_WIDTH - 160;

	//	プレイヤーの表示
	if (g_player.flg == TRUE) {
		if (JoyPadX < -300) {
			DrawRotaGraph(g_player.x, g_player.y, 2.3f, -M_PI / 18, players_img[0], TRUE, FALSE);
		}
		if (JoyPadX > 300) {
			DrawRotaGraph(g_player.x, g_player.y, 2.3f, -M_PI / 18, players_img[5], TRUE, FALSE);
		}
		else
		{
			DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, players_img[0], TRUE, FALSE);
		}

	}
	else {
		DrawRotaGraph(g_player.x, g_player.y, 0.3f, M_PI / 8 * (++g_player.count / 5), players_img[0], TRUE, FALSE);
		if (g_player.count >= 80)		g_player.flg = TRUE;
	}

	//	敵を避けた数を表示
	DrawBox(500, 0, 640, 480, 0x009900, TRUE);
	SetFontSize(16);
	//DrawFormatString(510, 20, 0x000000, "SCORE:%d",g_Score);
	DrawFormatString(540, 20, 0x000000, "残り時間");
	DrawFormatString(560, 60, 0x000000, "%d", timer/60);
	//DrawFormatString(560, 40, 0xFFFFFF, "%08d", g_Ranking[0].score);
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

	int dx1 = e->GetAppleX() - ((e->GetAppleW() / 2) - 20);
	int dy1 = e->GetAppleY() - (e->GetAppleH() / 2);
	int dx2 = dx1 + e->GetAppleW() - 50;
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