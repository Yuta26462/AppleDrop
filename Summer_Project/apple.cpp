#include "main.h"



int apple_count[4];				//リンゴを取得した個数

int apple_quantity = 0;			//描画されているリンゴの個数


void Apple::AppleControl() {
	int Made_apples = 0;			//生成するリンゴの個数
	static bool hitflg = false;
	static int apple_score = 0;		//あたったリンゴのスコアを格納する
	static int apple_type = 0;		//あたったリンゴの種類を格納する
	static int score_timer = 0;		//スコア表示用タイマー
	
	//時間ごとにリンゴ出現パターンを制御する
	if (GetTimeLimit() % 25 == 0 && apple_quantity <= APPLE_MAX)
	{
		//最初は4個生成
		if (GetStartFlg() == true) {
			CreateApple(APPLE_START);
			SetStartFlg(false);
		}
		else {
			Made_apples = (APPLE_MAX - apple_quantity) / 2;
			//切り上げ
			if ((APPLE_MAX - apple_quantity) % 2) {
				Made_apples++;
			}
			CreateApple(Made_apples);
		}

	}

	for (int i = 0; i < 11; i++)
	{
		if (apple[i].flg == true)
		{
			//敵の表示
			DrawRotaGraph(apple[i].x, apple[i].y, 1.0f, 0, apple[i].img, TRUE, FALSE);
			
			if (player.GetPlayerFlg() == FALSE)continue;

			//まっすぐ下に移動
			if (!isPause()) {
				apple[i].y += apple[i].speed;
				if (apple[i].y == 0) {
					apple_quantity++;
				}
			}
			

			//画面をはみ出したら消去
			if (apple[i].y > SCREEN_HEIGHT + apple[i].h) {
				apple[i].flg = false;
				apple[i].pos = 99;
				apple_quantity--;
			}

			//当たり判定
			if (player.HitBoxPlayer(&apple[i]) == TRUE && (player.GetStatus() == Poison_OFF))
			{
				apple[i].flg = false;
				apple[i].pos = 99;
				apple_quantity--;
				hitflg = true;
				apple_score = apple[i].score;
				apple_type = apple[i].type;
			
				SetMainScore(GetMainScore() + apple[i].score);
				apple_count[apple[i].type]++;
				if (apple[i].type == RED_APPLE) {
					PlaySoundMem(GetSound(SE_Red_AND_Green_Apple), DX_PLAYTYPE_BACK);

				}
				else if (apple[i].type == GREEN_APPLE) {
					PlaySoundMem(GetSound(SE_Red_AND_Green_Apple), DX_PLAYTYPE_BACK);
				}
				else if (apple[i].type == GOLD_APPLE) {
					PlaySoundMem(GetSound(SE_GoldenApple), DX_PLAYTYPE_BACK);
				}
				else if (apple[i].type == BLACK_APPLE) {
					PlaySoundMem(GetSound(SE_PoisonApple), DX_PLAYTYPE_BACK);
					player.SetStatus(Poison_ON);
				}
			}
			if (hitflg) {
				if(!isPause())score_timer = Timer(1, 3);
				if (score_timer < 180) {
					if (apple_type == BLACK_APPLE) {
						DrawFormatString(player.GetPlayerTransition(PlayerX) - 25, player.GetPlayerTransition(PlayerY) - 50, 0xff4000, "%d", apple_score);
					}
					else{
						DrawFormatString(player.GetPlayerTransition(PlayerX) - 25, player.GetPlayerTransition(PlayerY) - 50, 0xfffd3d, "+%d", apple_score);
					}
				}
				else {
					Timer(-1, 3);
					score_timer = 0;
					hitflg = false;
				}
			}
			
		}
	}

}

void Apple::CreateApple(int maxapple) {
	int apple_score[4] = { RED_SCORE,GREEN_SCORE,GOLD_SCORE,BLACK_SCORE };
	int made_apples = 0;
	//apple_firstcnt = apple_quantity;

	 for (int i = 0; i < 11; i++) {
		 if (made_apples < maxapple) {
			 if (apple[i].flg == false) {
				 made_apples++;
				 apple[i].flg = true;
				 apple[i].type = GetAppleType();
				 apple[i].img = GetAppleImage(apple[i].type);
				 apple[i].speed = GetAppleSpeed(apple[i].type);
				 apple[i].y = -50;
				 apple[i].pos = GetApplePos(apple[i].speed, i,&made_apples);
				 if (apple[i].pos == -1)break;
				 apple[i].x = apple[i].pos * 70 + 30;
				 
				 apple[i].w = 50;
				 apple[i].h = 50;
				 apple[i].score = apple_score[apple[i].type];

				 
				 //apple_quantity++;
			 }
		 }
	}
}

void Apple::AppleInit() {
	apple_quantity = 0;

	for (int i = 0; i < 11; i++) {
		if (apple[i].flg == true) {
			apple[i].flg = false;
			apple[i].pos = 99;
		}
	}
	for (int i = 0; i < 4; i++) {
		apple_count[i] = 0;
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
		return -1;
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
int Apple::GetApplePos(int apple_speed, int num ,int* made_apples) {
	int apple_pos = 0;
	int Over_flg = FALSE;			//リンゴが重なっているかのフラグ
	bool checkflg = false;			//重なったか調べたかのフラグ
	int old_position[7] = { 99,99,99,99,99,99,99 };
	int repeat_count = 0;

	apple_pos = GetRand(6);
	for (int i = 0; i < 11; i++) {
		
		if (apple[num].type == BLACK_APPLE && apple[i].type == BLACK_APPLE)apple[num].y -= 100;

		//位置が同じ既存のリンゴを調べる
		if (i != num && apple_pos == apple[i].pos && apple[num].speed > apple[i].speed) {
			Over_flg = TRUE;
			if (Over_flg < 0)break;
		}if (Over_flg) {
			apple[num].flg = false;
			made_apples--;
			break;
		}
	}		
	if (apple_pos < 7 && Over_flg >= 0) {
		return apple_pos;
	}
	else {
		return -1;
	}
}

int Apple::CheckAppleSpeed(int speed1, int speed2) {
	if (speed1 > speed2) {
		return TRUE;		//自身の速度が多い場合:重なる
	}
	else {
		return FALSE;		//自身の速度が遅いか同じ場合:重ならない
	}
	return -1;
}


bool Apple::GetAppleFlg() {
	return flg;
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

//int Apple::getpos() {
//	return pos;
//}
//
//int Apple::getspeed() {
//	return speed;
//}

int Apple::GetAppleCount(int type) {
	if (type == RED_APPLE) return apple_count[0];
	if (type == GREEN_APPLE) return apple_count[1];
	if (type == GOLD_APPLE) return apple_count[2];
	if (type == BLACK_APPLE) return apple_count[3];
	return -1;
}

