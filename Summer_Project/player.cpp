#include "main.h"

void Player::PlayerControl() {

	//	�R���̏���
	g_player.fuel -= g_player.speed;
	//	�Q�[���I�[�o�[������
	if (g_player.fuel <= 0)		g_GameState = 6;

	//	Z�L�[�ŉ���
	if (g_KeyFlg & PAD_INPUT_A && g_player.speed < 10)g_player.speed += 1;

	//	X�L�[�Ō���
	if (g_KeyFlg & PAD_INPUT_B && g_player.speed > 1)g_player.speed -= 1;

	//	�㉺���E�ړ�
	if (g_player.flg == TRUE) {
		if (g_NowKey & PAD_INPUT_UP)	g_player.y -= g_player.speed;
		if (g_NowKey & PAD_INPUT_DOWN)	g_player.y += g_player.speed;
		if (g_NowKey & PAD_INPUT_LEFT)	g_player.x -= g_player.speed;
		if (g_NowKey & PAD_INPUT_RIGHT)	g_player.x += g_player.speed;

	}

	//	��ʂ��͂ݏo���Ȃ��悤�ɂ���
	if (g_player.x < 32)		g_player.x = 32;

	if (g_player.x > SCREEN_WIDTH - 180)		g_player.x = SCREEN_WIDTH - 180;
	if (g_player.y < 60)		g_player.y = 60;
	if (g_player.y > SCREEN_HEIGHT - 60)			g_player.y = SCREEN_HEIGHT - 60;

	//	�v���C���[�̕\��
	if (g_player.flg == TRUE) {
		if (g_NowKey & PAD_INPUT_LEFT) {
			DrawRotaGraph(g_player.x, g_player.y, 1.0f, -M_PI / 18, g_Car, TRUE, FALSE);
		}
		else if (g_NowKey & PAD_INPUT_RIGHT) {
			DrawRotaGraph(g_player.x, g_player.y, 1.0f, -M_PI / 18, g_Car, TRUE, FALSE);
		}
		else
		{
			DrawRotaGraph(g_player.x, g_player.y, 1.0f, 0, g_Car, TRUE, FALSE);
		}

		//�o���A

		if (g_KeyFlg & PAD_INPUT_C && g_player.bari > 0 && g_player.baricnt <= 0) {
			g_player.bari--;
			g_player.baricnt = 1000;
		}
		if (g_player.baricnt > 0) {
			g_player.baricnt -= g_player.speed;
			DrawRotaGraph(g_player.x, g_player.y, 1.0f, 0, g_Barrier, TRUE, FALSE);
		}
		else { g_player.baricnt = 0; }

	}
	else {
		DrawRotaGraph(g_player.x, g_player.y, 1.0f, M_PI / 8 * (++g_player.count / 5), g_Car, TRUE, FALSE);
		if (g_player.count >= 80)		g_player.flg = TRUE;
	}

	//	�G�����������\��
	SetFontSize(16);
	DrawFormatString(510, 20, 0x000000, "�n�C�X�R�A");
	DrawFormatString(560, 40, 0xFFFFFF, "%08d", g_Ranking[0].score);
	DrawFormatString(510, 80, 0x000000, "��������");
	DrawRotaGraph(523, 120, 0.3f, 0, g_Teki[0], TRUE, FALSE);
	DrawRotaGraph(573, 120, 0.3f, 0, g_Teki[1], TRUE, FALSE);
	DrawRotaGraph(623, 120, 0.3f, 0, g_Teki[2], TRUE, FALSE);
	DrawRotaGraph(623, 180, 0.3f, 0, g_Bike[0], TRUE, FALSE);	//�`�������W5

	DrawFormatString(510, 140, 0xFFFFFF, "%03d", g_EnemyCount1);
	DrawFormatString(560, 140, 0xFFFFFF, "%03d", g_EnemyCount2);
	DrawFormatString(610, 140, 0xFFFFFF, "%03d", g_EnemyCount3);
	DrawFormatString(610, 200, 0xFFFFFF, "%03d", g_EnemyCount4);	//�`�������W5
	DrawFormatString(510, 200, 0x000000, "���s����");
	DrawFormatString(555, 220, 0xFFFFFF, "%08d", g_Mileage / 10);
	DrawFormatString(510, 240, 0x000000, "�X�s�[�h");
	DrawFormatString(555, 260, 0xFFFFFF, "%08d", g_player.speed);

	//	�o���A�̕\��
	for (int i = 0; i < g_player.bari; i++) {
		DrawRotaGraph(520 + i * 25, 340, 0.2f, 0, g_Barrier, TRUE, FALSE);
	}

	//	�R���Q�[�W�̕\��
	int F_X = 510; int F_Y = 390; int F_W = 100; int F_H = 20;
	DrawString(F_X, F_Y, "FUEL METER", 0x000000, 0);
	//	�����̃Q�[�W
	DrawBox(F_X, F_Y + 20, F_X + (int)(g_player.fuel * F_W / PLAYER_FUEL), F_Y + 20 + F_H, 0x0066cc, TRUE);
	//	�O���̃Q�[�W
	DrawBox(F_X, F_Y + 20, F_X + F_W, F_Y + 20 + F_H, 0x000000, FALSE);


	//	�̗̓Q�[�W�̕\��
	int X = 510; int Y = 430; int W = 100; int H = 20;
	DrawString(X, Y, "PLAYER HP", 0x000000, 0);
	//	�����̃Q�[�W
	DrawBox(X, Y + 20, X + (int)(g_player.hp * W / PLAYER_HP), Y + 20 + H, 0xff0000, TRUE);
	//	�O���̃Q�[�W
	; DrawBox(X, Y + 20, X + W, Y + 20 + H, 0x000000, FALSE);
	DrawFormatString(100, 200, 0x000000, "%d/10%%50", g_Mileage);
	DrawFormatString(100, 300, 0x000000, "%d", g_Mileage / 10 % 50);
}

int Player::KeyInput(void){
	g_OldKey = g_NowKey;
	g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	g_KeyFlg = g_NowKey & ~g_OldKey;

	return g_KeyFlg;
}