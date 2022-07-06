#include "main.h"


int apple_score[4] = { RED_SCORE,GREEN_SCORE,GOLD_SCORE,BLACK_SCORE };
int apple_quantity = 0;			//�`�悳��Ă��郊���S�̌�
int apple_count[4];

void Apple::AppleControl(bool pauseflg) {
	for (int i = 0; i < APPLE_MAX; i++)
	{
		if (apple[i].flg == true)
		{
			//�G�̕\��
			DrawRotaGraph(apple[i].x, apple[i].y, 1.0f, 0, apple[i].img, TRUE, FALSE);

			if (g_player.flg == FALSE)continue;

			//�܂��������Ɉړ�
			if (!pauseflg) {
				apple[i].y += apple[i].speed;
			}

			//��ʂ��͂ݏo���������
			if (apple[i].y > SCREEN_HEIGHT + apple[i].h) {
				apple[i].flg = false;
				apple_quantity--;
			}

			//�����蔻��
			if (HitBoxPlayer(&g_player, &apple[i]) == TRUE)
			{
				apple[i].flg = false;
				apple_quantity--;
				g_Score += apple[i].score;
				apple_count[apple[i].type]++;
			}
		}
	}

	

	//���s�������ƂɓG�o���p�^�[���𐧌䂷��
	if (timer % 25 == 0 && (APPLE_MAX - apple_quantity) / 2 > apple_quantity)
	{
		if (StartFlg == true) {
				apple_quantity++;
				CreateApple(APPLE_START);
				StartFlg = false;
		}
		else {
			apple_quantity++;
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
			apple[i].x = GetRand(6) * 70 + 30;
			apple[i].y = -50;
			apple[i].w = 60;
			apple[i].h = 60;
			apple[i].speed = GetAppleSpeed(apple[i].type);
			apple[i].score = apple_score[apple[i].type];
			//	����
			return TRUE;
		}
	}
	//	���s
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