#include "main.h"

extern Menu menu;


extern int g_GameState;
int g_TitleImage;
int g_Menu, g_Cone;

int g_Score = 0, g_RankingImage;

int LoadImages();

int g_Item[2];

int g_WaitTime = 0;
int g_EndImage;

int g_Mileage;	//走行距離y
int g_EnemyCount1, g_EnemyCount2, g_EnemyCount3;
int g_EnemyCount4; //	チャレンジ5
int g_Teki[3];	//キャラ画像変数

int g_StageImage;
int g_Car, g_Barrier;

int spflag;		//チャレンジ4用フラグ
int g_Bike[1];		//チャレンジ5	バイク画像変数
int bikec = 0;

struct ENEMY g_enemy00 = { TRUE,0,0,0,-50,63,120,0,1 };
struct ENEMY g_enemyCn = { TRUE,4,0,0,-50,18,18,0,1 };
struct ENEMY g_item00 = { TRUE,0,0,0,-50,50,50,0,1 };
int g_GameState = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	SetMainWindowText("Drive&Avoid");
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	if ((g_RankingImage = LoadGraph("images/Ranking.png")) == -1)return-1;
	if (LoadImages() == -1)return -1;

	while (ProcessMessage() == 0 && g_GameState != 99 && !(player.KeyInput() & PAD_INPUT_START)) {

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
		case 4:
			DrawEnd();
			break;
		case 5:
			GameMain();
			break;
		case 6:
			DrawGameOver();
			break;
		case 7:
			ranking.InputRanking();
			break;
		}
		ScreenFlip();
	}
	DxLib_End();

	return 0;
}


void DrawGameTitle(void) {
	static int MenuNo = 0;

	if (player.KeyInput() & PAD_INPUT_DOWN)if (++MenuNo > 3)MenuNo = 0;
	if (player.KeyInput() & PAD_INPUT_UP)if (--MenuNo < 0)MenuNo = 3;

	// Zキーでメニュー選択
	if (player.KeyInput() & PAD_INPUT_A)g_GameState = MenuNo + 1;

	DrawGraph(0, 0, g_TitleImage, FALSE);
	DrawGraph(120, 200, g_Menu, TRUE);
	DrawRotaGraph(90, 220 + MenuNo * 40, 0.7f, M_PI / 2, g_Cone, TRUE);

}

void GameInit(void) {
	g_Score = 0;
	spflag = 0;		//チャレンジ5用
	bikec = 0;		//チャレンジ5用

	//	走行距離を初期化
	g_Mileage = 0;

	//	敵1を避けた数の初期設定
	g_EnemyCount1 = 0;
	g_EnemyCount2 = 0;
	g_EnemyCount3 = 0;
	g_EnemyCount4 = 0;

	//	プレイヤーの初期設定
	g_player.flg = TRUE;
	g_player.x = PLAYER_POS_X;
	g_player.y = PLAYER_POS_Y;
	g_player.w = PLAYER_WIDTH;
	g_player.h = PLAYER_HEIGHT;
	g_player.angle = 0.0;
	g_player.count = 0;
	g_player.speed = PLAYER_SPEED;
	g_player.hp = PLAYER_HP;
	g_player.fuel = PLAYER_FUEL;
	g_player.bari = PLAYER_BARRIER;
	g_player.bariup = PLAYER_BARRIERUP;


	g_GameState = 5;

	for (int i = 0; i < ENEMY_MAX; i++) {
		g_enemy[i].flg = FALSE;
	}
	for (int i = 0; i < ENEMY_MAX; i++) {	//チャレンジ5
		g_enemy2[i].flg = FALSE;
	}
	for (int i = 0; i < ITEM_MAX; i++) {
		g_item[i].flg = FALSE;
	}
}

void DrawHelp(void) {
	if (player.KeyInput() & PAD_INPUT_M)	g_GameState = 0;

	DrawGraph(0, 0, g_TitleImage, FALSE);
	SetFontSize(16);
	DrawString(20, 120, "ヘルプ画面", 0xffffff, 0);

	DrawString(20, 160, "これは障害物を避けながら", 0xffffff, 0);
	DrawString(20, 180, "走り続けるゲームです", 0xffffff, 0);
	DrawString(20, 200, "燃料が尽きるか障害物に", 0xffffff, 0);
	DrawString(20, 220, "数回当たるとゲームオーバーです", 0xffffff, 0);
	DrawString(20, 250, "アイテム一覧", 0xffffff, 0);
	DrawGraph(20, 260, g_Item[0], TRUE);
	DrawString(20, 315, "取ると燃料が回復するよ。", 0xffffff, 0);
	DrawGraph(20, 335, g_Item[1], TRUE);
	DrawString(20, 385, "ダメージを受けている時に取ると耐久回復", 0xffffff, 0);
	DrawString(20, 405, "耐久が減っていなかったら燃料が少し回復するよ。", 0xffffff, 0);
	DrawString(20, 450, "---- スペースキーを押してタイトルへ戻る ----", 0xffffff, 0);
}

void DrawEnd(void) {
	DrawGraph(0, 0, g_EndImage, FALSE);

	SetFontSize(24);
	DrawString(360, 480 - 24, "Thank you for Playing", 0xffffff, 0);

	//	タイムの加算処理&終了（3秒後）
	if (++g_WaitTime > 180)g_GameState = 99;
}

void GameMain(void) {
	//if (g_KeyFlg & PAD_INPUT_M)g_GameState = 6;	//テスト用の処理！？

	//SetFontSize(16);
	//DrawString(20, 20, "ゲームメイン", 0xffffff, 0);
	//DrawString(150, 450, "---- スペースキーを押してゲームオーバーへ ----", 0xffffff, 0);

	BackScrool();

	EnemyControl();
	g_enemy2->h = 100;	//	チャレンジ5
	BikeControl();

	ItemControl();

	player.PlayerControl();
}

void DrawGameOver(void) {


	BackScrool();

	spflag = 1;

	g_Score = (g_Mileage / 10 * 10) + g_EnemyCount4 * 300 + g_EnemyCount3 * 50 + g_EnemyCount2 * 100 + g_EnemyCount1 * 200;

	if (player.KeyInput() & PAD_INPUT_M) {
		if (g_Ranking[RANKING_DATA - 1].score >= g_Score) {
			g_GameState = 0;
		}
		else {
			g_GameState = 7;
		}
	}


	//g_Mileage = g_Score;

	DrawBox(150, 150, 490, 330, 0x009900, TRUE);
	DrawBox(150, 150, 490, 330, 0x000000, FALSE);

	SetFontSize(20);
	DrawString(220, 170, "ゲームオーバー", 0xcc0000);
	SetFontSize(16);
	DrawString(180, 200, "走行距離     ", 0x000000);
	DrawRotaGraph(230, 230, 0.3f, M_PI / 2, g_Teki[2], TRUE, FALSE);
	DrawRotaGraph(230, 250, 0.3f, M_PI / 2, g_Teki[1], TRUE, FALSE);
	DrawRotaGraph(230, 270, 0.3f, M_PI / 2, g_Teki[0], TRUE, FALSE);
	DrawRotaGraph(230, 290, 0.3f, M_PI / 2, g_Bike[0], TRUE, FALSE);



	DrawFormatString(260, 200, 0xFFFFFF, " %6d x  10 = %6d", g_Mileage / 10, g_Mileage / 10 * 10);
	DrawFormatString(260, 285, 0xFFFFFF, " %6d x 300 = %6d", g_EnemyCount4, g_EnemyCount4 * 300);
	DrawFormatString(260, 222, 0xFFFFFF, " %6d x  50 = %6d", g_EnemyCount3, g_EnemyCount3 * 50);
	DrawFormatString(260, 243, 0xFFFFFF, "%6d x  100 = %6d", g_EnemyCount2, g_EnemyCount2 * 100);
	DrawFormatString(260, 264, 0xFFFFFF, "%6d x  200 = %6d", g_EnemyCount1, g_EnemyCount1 * 200);

	DrawString(280, 310, "スコア ", 0x000000);
	DrawFormatString(310, 310, 0xFFFFFF, "         = %6d", g_Score);
	DrawString(150, 450, "---- スペースキーを押してタイトルへ戻る ----", 0xffffff, 0);

}



int LoadImages() {
	if ((g_TitleImage = LoadGraph("images/Title.png")) == -1)return-1;
	if ((g_Menu = LoadGraph("images/menu.bmp")) == -1)return-1;
	if ((g_Cone = LoadGraph("images/cone.bmp")) == -1)return-1;
	if ((g_Item[0] = LoadGraph("images/greenapple.png")) == -1)return-1;
	if ((g_Item[1] = LoadGraph("images/apple.png")) == -1)return-1;
	if ((g_EndImage = LoadGraph("images/background.png")) == -1)return-1;
	if (LoadDivGraph("images/car.bmp", 3, 3, 1, 63, 120, g_Teki) == -1)return -1;	//敵
	if ((g_StageImage = LoadGraph("images/pause.png")) == -1)return-1;
	if ((g_Car = LoadGraph("images/purpleapple.png")) == -1)return-1;
	if ((g_Barrier = LoadGraph("images/barrier.png")) == -1)return-1;
	if (LoadDivGraph("images/bike.bmp", 1, 1, 1, 32, 72, g_Bike) == -1)return -1;
	//if ((g_Bike = LoadGraph("images/05/bike.bmp")) == -1)return-1;
	return 0;
}



void BackScrool()
{
	//g_Mileage += 5;
	//if (!spflag) { g_Mileage += g_player.speed; }
	if (g_GameState != 6)g_Mileage += g_player.speed;

	//ステージ描画表示

	SetDrawArea(0, 0, 500, 480);
	DrawGraph(0, g_Mileage % 480 - 480, g_StageImage, FALSE);
	DrawGraph(0, g_Mileage % 480, g_StageImage, FALSE);

	SetDrawArea(0, 0, 640, 480);

	DrawBox(500, 0, 640, 480, 0x009900, TRUE);
}

void EnemyControl() {
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (g_enemy[i].flg == TRUE) {

			//	敵の表示
			DrawRotaGraph(g_enemy[i].x, g_enemy[i].y, 1.0f, 0, g_enemy[i].img, TRUE, FALSE);
			if (g_player.flg == FALSE)continue;

			//	真っ直ぐ下に移動
			g_enemy[i].y += g_enemy[i].speed + g_player.speed - PLAYER_SPEED + 1;

			//	画面をはみ出しだしたら消去
			if (g_enemy[i].y > SCREEN_HEIGHT + g_enemy[i].h)g_enemy[i].flg = FALSE;

			//	敵機を追い越したらカウントする
			if (g_enemy[i].y > g_player.y && g_enemy[i].point == 1) {
				g_enemy[i].point = 0;
				if (g_enemy[i].type == 0)g_EnemyCount1++;
				if (g_enemy[i].type == 1)g_EnemyCount2++;
				if (g_enemy[i].type == 2)g_EnemyCount3++;
			}

			/*if (g_enemy2[i].y > g_player.y && g_enemy2[i].point == 1) {
				g_enemy2[i].point = 0;
				if (g_enemy2[i].type == 0)g_EnemyCount1++;
			}*/

			//	当たり判定
			if (HitBoxPlayer(&g_player, &g_enemy[i]) == TRUE && g_player.baricnt <= 0) {
				g_player.flg = FALSE;
				g_player.speed = PLAYER_SPEED;
				g_player.count = 0;
				g_player.hp -= 100;
				g_enemy[i].flg = FALSE;
				if (g_player.hp <= 0)		g_GameState = 6;
			}
		}
	}

	//	歩行距離ごとに敵出現パターンを制御する
	if (g_Mileage / 10 % 50 == 0) {
		CreateEnemy();
	}
}

void BikeControl() {
	bikec++;	//チャレンジ5
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (g_enemy2[i].flg == TRUE) {

			//	敵の表示
			DrawRotaGraph(g_enemy2[i].x, g_enemy2[i].y, 1.0f, 0, g_enemy2[i].img, TRUE, FALSE);
			if (g_player.flg == FALSE)continue;

			//	真っ直ぐ下に移動
			//DrawFormatString(540, 400, 0xFFFFFF, "count:%d" , bikec); //カウントチェック用
			int bikec2 = bikec / 1000 % 2 == 1;
			if (bikec < 300)g_enemy2[i].x += g_enemy2[i].speed + g_player.speed - PLAYER_SPEED + 1;		//	チャレンジ5
			else if (bikec < 600)g_enemy2[i].x -= g_enemy2[i].speed + g_player.speed - PLAYER_SPEED + 1;		//	チャレンジ5
			else bikec = 0;

			g_enemy2[i].y += g_enemy2[i].speed + g_player.speed - PLAYER_SPEED + 1;

			//	画面をはみ出しだしたら消去
			if (g_enemy2[i].y > SCREEN_HEIGHT + g_enemy2[i].h)g_enemy2[i].flg = FALSE;
			if (g_enemy2[i].x + 215 > SCREEN_WIDTH + g_enemy2[i].w)g_enemy2[i].flg = FALSE;		//チャレンジ5;

			//	敵機を追い越したらカウントする
			if (g_enemy2[i].y > g_player.y && g_enemy2[i].point == 1) {
				g_enemy2[i].point = 0;
				if (g_enemy2[i].type == 0)g_EnemyCount4++;
			}


			//	当たり判定
			if (HitBoxPlayer(&g_player, &g_enemy2[i]) == TRUE && g_player.baricnt <= 0) {
				g_player.flg = FALSE;
				g_player.speed = PLAYER_SPEED;
				g_player.count = 0;
				g_player.hp -= 50;
				g_enemy2[i].flg = FALSE;
				if (g_player.hp <= 0)		g_GameState = 6;
			}
		}
	}

	//	歩行距離ごとに敵出現パターンを制御する
	if (g_Mileage / 10 % 50 == 0) {
		CreateBike();
	}
}

int CreateEnemy() {

	for (int i = 0; i < ENEMY_MAX; i++) {
		if (g_enemy[i].flg == FALSE) {
			g_enemy[i] = g_enemy00;
			g_enemy[i].type = GetRand(2);
			g_enemy[i].img = g_Teki[g_enemy[i].type];
			g_enemy[i].x = GetRand(4) * 105 + 40;
			g_enemy[i].speed = g_enemy[i].type * 2;

			//	成功
			return TRUE;
		}
	}

	//	失敗
	return FALSE;

}

int CreateBike() {

	for (int i = 0; i < ENEMY_MAX; i++) {
		if (g_enemy2[i].flg == FALSE) {
			g_enemy2[i] = g_enemy00;
			g_enemy2[i].type = GetRand(2);
			g_enemy2[i].img = g_Bike[g_enemy2[i].type];
			g_enemy2[i].x = GetRand(4) * 105 + 40;
			g_enemy2[i].speed = g_enemy2[i].type * 2;

			//	成功
			return TRUE;
		}
	}

	//	失敗
	return FALSE;

}

int HitBoxPlayer(PLAYER* p, ENEMY* e) {

	//	x,y は中心座標とする
	int sx1 = p->x - (p->w / 2);
	int sy1 = p->y - (p->h / 2);
	int sx2 = sx1 + p->w;
	int sy2 = sy1 + p->h;

	int dx1 = e->x - ((e->w / 2) - 20);
	int dy1 = e->y - (e->h / 2);
	int dx2 = dx1 + e->w - 50;
	int dy2 = dy1 + e->h;

	//	短形が重なっていれば当たり
	if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
		return TRUE;
	}
	return FALSE;
}


void ItemControl() {
	for (int i = 0; i < ITEM_MAX; i++) {
		if (g_item[i].flg == TRUE) {
			//	アイテムの表示
			DrawRotaGraph(g_item[i].x, g_item[i].y, 1.0f, 0, g_item[i].img, TRUE);

			if (g_player.flg == FALSE)continue;

			//	真っすぐ下に移動
			g_item[i].y += g_item[i].speed + g_player.speed - PLAYER_SPEED;

			//	画面をはみ出したら消去
			if (g_item[i].y > SCREEN_HEIGHT)g_item[i].flg = FALSE;

			if (HitBoxPlayer(&g_player, &g_item[i]) == TRUE) {
				g_item[i].flg = FALSE;
				if (g_item[i].type == 0)g_player.fuel += g_item[i].point;
				if (g_item->type == 1) {
					g_player.hp += g_item[i].point;
					if (g_player.hp > PLAYER_HP)g_player.hp = PLAYER_HP;
				}
			}
		}
	}

	//	走行距離ごとに敵出現パターンを制御する
	if (g_Mileage / 10 % 500 == 0)CreateItem();
}

int CreateItem() {

	for (int i = 0; i < ITEM_MAX; i++) {
		if (g_item[i].flg == FALSE) {
			g_item[i] = g_item00;
			g_item[i].type = GetRand(1);
			g_item[i].img = g_Item[g_item[i].type];
			g_item[i].x = GetRand(4) * 105 + 40;
			g_item[i].speed = 1 + g_item[i].type * 3;
			if (g_item[i].type == 0)	g_item[i].point = 500;
			if (g_item[i].type == 1)	g_item[i].point = 50;

			//	成功
			return TRUE;
		}
	}

	// 失敗
	return FALSE;
}