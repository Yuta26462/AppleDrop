#include "main.h"


int apple_score[4] = { RED_SCORE,GREEN_SCORE,GOLD_SCORE,BLACK_SCORE };
int apple_quantity = 0;			//•`‰æ‚³‚ê‚Ä‚¢‚éƒŠƒ“ƒS‚ÌŒÂ”
int apple_count[4];



void Apple::AppleControl(bool pauseflg) {
	DrawFormatString(100, 400, 0x000000, "aplle_quantity:%d", apple_quantity);
	for (int i = 0; i < APPLE_MAX; i++)
	{
		if (apple[i].flg == true)
		{
			//“G‚Ì•\¦
			DrawRotaGraph(apple[i].x, apple[i].y, 1.0f, 0, apple[i].img, TRUE, FALSE);

			if (g_player.flg == FALSE)continue;

			//‚Ü‚Á‚·‚®‰º‚ÉˆÚ“®
			if (!pauseflg) {
				apple[i].y += apple[i].speed;
			}

			//‰æ–Ê‚ğ‚Í‚İo‚µ‚½‚çÁ‹
			if (apple[i].y > SCREEN_HEIGHT + apple[i].h) {
				apple[i].flg = false;
				apple[i].pos = 99;
				apple_quantity--;
			}

			//“–‚½‚è”»’è
			if (HitBoxPlayer(&g_player, &apple[i]) == TRUE && !g_player.Poisonflg)
			{
				apple[i].flg = false;
				apple[i].pos = 99;
				apple_quantity--;
				g_Score += apple[i].score;
				apple_count[apple[i].type]++;
				if (apple[i].type == BLACK_APPLE) {
					g_player.Poisonflg = true;
				}
			}
		}
	}

	

	//ŠÔ‚²‚Æ‚ÉƒŠƒ“ƒSoŒ»ƒpƒ^[ƒ“‚ğ§Œä‚·‚é
	if (timer % 25 == 0 && (APPLE_MAX - apple_quantity) / 2 > apple_quantity)
	{
		if (StartFlg == true) {	
			CreateApple(APPLE_START);
			StartFlg = false;
		}
		else {
			CreateApple(APPLE_MAX);
		}
		
	}
}

int Apple::CreateApple(int maxapple) {
	for (int i = 0; i < maxapple; i++) {
		if (apple[i].flg == false) {
			apple[i].flg = true;
			apple[i].type = GetAppleType();
			apple[i].img = apple_img[apple[i].type];
			apple[i].speed = GetAppleSpeed(apple[i].type);
			apple[i].pos = GetApplePos(apple[i].speed, i);
			apple[i].x = apple[i].pos * 70 + 30;//GetRand(6) * 70 + 30;
			apple[i].y = -50;
			apple[i].w = 50;
			apple[i].h = 50;
			apple[i].score = apple_score[apple[i].type];
			apple_quantity++;
			//	¬Œ÷
			return TRUE;
		}
	}
	//	¸”s
	return FALSE;
}

void Apple::AppleInit() {
	apple_quantity = 0;

	for (int i = 0; i < APPLE_MAX; i++) {
		if (apple[i].flg == true) {
			apple[i].flg = false;
			
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
		break;
	}
}

int Apple::GetApplePos(int apple_speed, int num) {
	int apple_pos = 0;
	bool Over_flg = true;

	if (Over_flg == TRUE) {
		for (int i = 0; i < APPLE_MAX; i++) {
			if (i != num) {
				apple_pos = GetRand(6);
				if (apple_pos == apple[i].pos) {
					CheckAppleSpeed(apple[num].speed, apple[i].speed, &Over_flg);
				}
			}
		}
	}

	return apple_pos;
}

void Apple::CheckAppleSpeed(int speed1, int speed2, bool* over_flg) {
	if (speed1 > speed2) {
		*over_flg = true;
	}
	else {
		*over_flg = false;
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