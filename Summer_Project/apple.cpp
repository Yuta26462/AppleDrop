#include "main.h"

bool apple_flg;
int apple_score[4] = { RED_SCORE,GREEN_SCORE,GOLD_SCORE,BLACK_SCORE };
int timer = 0;

void Apple::AppleControl() {
	for (int i = 0; i < APPLE_MAX; i++)
	{
		if (apple[i].flg == true)
		{
			//�G�̕\��
			DrawRotaGraph(apple[i].x, apple[i].y, 1.0f, 0, apple[i].img, TRUE, FALSE);

			if (g_player.flg == FALSE)continue;

			//�܂��������Ɉړ�
			apple[i].y += apple[i].speed + g_player.speed - PLAYER_SPEED + 1;

			//��ʂ��͂ݏo���������
			if (apple[i].y > SCREEN_HEIGHT + apple[i].h)	apple[i].flg = false;

			////�G�@��ǂ��z������J�E���g����
			//if (g_enemy[i].y > g_player.y && g_enemy[i].point == 1)
			//{
			//	g_enemy[i].point = 0;
			//	if (g_enemy[i].type == 0)g_EnemyCount1++;
			//	if (g_enemy[i].type == 1)g_EnemyCount2++;
			//	if (g_enemy[i].type == 2)g_EnemyCount3++;

			//}



			//�����蔻��
			if (HitBoxPlayer(&g_player, &apple[i]) == TRUE && g_player.baricnt <= 0)
			{
				g_player.flg = FALSE;
				g_player.speed = PLAYER_SPEED;
				g_player.count = 0;
				g_player.hp -= 100;
				apple[i].flg = false;
				if (g_player.hp <= 0) g_GameState = 6;
			}
		}
	}

	//���s�������ƂɓG�o���p�^�[���𐧌䂷��
	if (++timer % 25== 0)
	{
		CreateApple();
	}
}

int Apple::CreateApple() {
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (apple[i].flg == false) {
			apple[i].flg = true;
			apple[i].type = GetAppleType();
			apple[i].img = apple_img[apple[i].type];
			apple[i].x = GetRand(4) * 105 + 40;
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
	/*else if (apple_type == 9) {
		return BLACK_APPLE;
	}*/
	else {
		return NULL;
	}
}

int GetAppleSpeed(int AppleSpeed) {
	switch (AppleSpeed) {
	case RED_APPLE:
		return 2;
		break;
	case GREEN_APPLE:
		return 5;
		break;
	case GOLD_APPLE:
		return 10;
		break;
	/*case BLACK_APPLE:
		return 1;
		break;*/
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