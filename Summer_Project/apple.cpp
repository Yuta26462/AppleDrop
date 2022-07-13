#include "main.h"


int apple_score[4] = { RED_SCORE,GREEN_SCORE,GOLD_SCORE,BLACK_SCORE };
int apple_count[4];				//リンゴを取得した個数
bool Over_flg = false;			//リンゴが重なっているかのフラグ
int apple_quantity = 0;			//描画されているリンゴの個数
int Made_apples = 0;			//生成するリンゴの個数



void Apple::AppleControl(bool pauseflg) {
	//時間ごとにリンゴ出現パターンを制御する
	if (timer % 25 == 0 && apple_quantity <= APPLE_MAX)
	{
		//最初は4個生成
		if (StartFlg == true) {
			CreateApple(APPLE_START);
			StartFlg = false;
		}
		else {
			Made_apples = (APPLE_MAX - apple_quantity) / 2;
			//切り上げ
			if (Made_apples % 2) {
				Made_apples++;
			}
			CreateApple(Made_apples);
		}

	}

	for (int i = 0; i < APPLE_MAX; i++)
	{
		if (apple[i].flg == true)
		{
			//敵の表示
			DrawRotaGraph(apple[i].x, apple[i].y, 1.0f, 0, apple[i].img, TRUE, FALSE);

			if (g_player.flg == FALSE)continue;

			//まっすぐ下に移動
			if (!pauseflg) {
				apple[i].y += apple[i].speed;
			}

			//画面をはみ出したら消去
			if (apple[i].y > SCREEN_HEIGHT + apple[i].h) {
				apple[i].flg = false;
				apple[i].pos = 99;
				apple_quantity--;
			}

			//当たり判定
			if (HitBoxPlayer(&g_player, &apple[i]) == TRUE && !g_player.Poisonflg)
			{
				apple[i].flg = false;
				apple[i].pos = 99;
				apple_quantity--;
				g_Score += apple[i].score;
				apple_count[apple[i].type]++;
				if (apple[i].type == BLACK_APPLE) {
					PlaySoundMem(PoisonApple_SE, DX_PLAYTYPE_BACK);
					g_player.Poisonflg = true;
				}
				if (apple[i].type == GOLD_APPLE) {
					PlaySoundMem(GoldenApple_SE, DX_PLAYTYPE_BACK);
				}
				if (apple[i].type == RED_APPLE || GREEN_APPLE) {
					PlaySoundMem(Red_AND_Green_Apple_SE, DX_PLAYTYPE_BACK);
				}
			}
		}
	}

}

int Apple::CreateApple(int maxapple) {
	
	int apple_count = 0;
	apple_count = apple_quantity;
	 for (int i = 0; i < APPLE_MAX; i++) {
		 if ((apple_quantity - apple_count) < maxapple) {
			 if (apple[i].flg == false) {
				 apple[i].flg = true;
				 apple[i].type = GetAppleType();
				 apple[i].img = apple_img[apple[i].type];
				 apple[i].speed = GetAppleSpeed(apple[i].type);
				 apple[i].pos = GetApplePos(apple[i].speed, i);
				 apple[i].x = apple[i].pos * 70 + 30;/*GetRand(6) * 70 + 30;*///
				 apple[i].y = -50;
				 apple[i].w = 50;
				 apple[i].h = 50;
				 apple[i].score = apple_score[apple[i].type];

					 apple_quantity++;
				 //	成功
				 return TRUE;
			 }
		 }
	}

	//	失敗
	return FALSE;
}

void Apple::AppleInit() {
	apple_quantity = 0;

	for (int i = 0; i < APPLE_MAX; i++) {
		if (apple[i].flg == true) {
			apple[i].flg = false;
			apple[i].pos = 99;
		}
	}
}

int GetAppleType() {
	int apple_type = 0;
	apple_type = GetRand(9);
	if (apple_type <= 5) {
		return RED_APPLE;
	}
	else if (apple_type > 5 && apple_type <= 7) {
		return GREEN_APPLE;
	}
	else if (apple_type == 8) {
		return GOLD_APPLE;
	}
	else if (apple_type == 9) {
		return BLACK_APPLE;
	}
	else {
		return NULL;
	}
}

int GetAppleSpeed(int AppleType) {
	switch (AppleType) {
	case RED_APPLE:
		return RED_SPEED;
		break;
	case GREEN_APPLE:
		return GREEN_SPEED;
		break;
	case GOLD_APPLE:
		return GOLD_SPEED;
		break;
	case BLACK_APPLE:
		return BLACK_SPEED;
		break;
	default:
		return -1;
		break;
	}
}

//リンゴの出現場所を決める
int Apple::GetApplePos(int apple_speed, int num) {
	int apple_pos = 0;
	bool checkflg = false;
	int old_position[7] = { 99,99,99,99,99,99,99 };
	int repeat_count = 0;
	int j;

	apple_pos = GetRand(6);
	for (int i = 0; i < APPLE_MAX; i++) {
		if (repeat_count > 6) {
			apple[num].flg = false;
			break;
		}
		if (Over_flg == TRUE && checkflg == TRUE && repeat_count <= 6) {		//重なった場合、位置を決め直す
			j = 0;
			apple_pos = GetRand(6);
			checkflg = false;
			old_position[repeat_count] = apple[i - 1].pos;
			while (apple_pos != old_position[j] && j < 7) {
				apple_pos = GetRand(6);
				j++;
			}
			repeat_count++;
			i = 0;
		}
		//位置が同じ既存のリンゴを調べる
		if (i != num && apple_pos == apple[i].pos) {
			CheckAppleSpeed(apple[num].speed, apple[i].speed);	
			if (!checkflg) {
				checkflg = true;
			}
		}
	}		
				return apple_pos;
	
}

void Apple::CheckAppleSpeed(int speed1, int speed2) {
	if (speed1 > speed2) {
		Over_flg = true;		//自身の速度が多い場合:重なる
	}
	else {
		Over_flg = false;		//自身の速度が遅いか同じ場合:重ならない
	}
}


bool* Apple::GetAppleFlg() {
	return &flg;
}

int Apple::GetAppleX() {
	return x;
}

int Apple::GetAppleY() {
	return y;
}

int Apple::GetAppleW() {
	return w;
}

int Apple::GetAppleH() {
	return h;
}

int Apple::getpos() {
	return pos;
}