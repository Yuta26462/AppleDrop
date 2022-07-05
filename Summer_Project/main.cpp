#include "main.h"

#define TIMELIMIT 300 + 60

Apple apple[APPLE_MAX];
Apple AppleFunc;
int g_OldKey, g_NowKey, g_KeyFlg;
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
int g_Menu, g_Cone;
int g_PosY, gPosX;
int g_WaitTime = 0;
int g_EndImage;
int g_StageImage;
bool apple_flg;
int apple_x;
int apple_y;

int LoadImages();
int LoadSounds();

//�T�E���h�p�ϐ�
int TitleBGM;
int GameMainBGM;
int RankingBGM;
int EndBGM;
int SE;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	SetMainWindowText("Drive&Avoid");
	ChangeWindowMode(TRUE);

	double dNextTime = GetNowCount();

	if (DxLib_Init() == -1)return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	if ((g_RankingImage = LoadGraph("images/Ranking.png")) == -1)return-1;
	if (LoadImages() == -1)return -1;
	if (LoadSounds() == -1)return -1;

	while (ProcessMessage() == 0 && g_GameState != 99 && !(g_KeyFlg & PAD_INPUT_START)) {

		g_OldKey = g_NowKey;
		g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		g_KeyFlg = g_NowKey & ~g_OldKey;

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
			DrawGameOver();
			break;
		case 7:
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

		//�^�C�g��BGM���X�^�[�g
	if (CheckSoundMem(TitleBGM) == 0)
	{
		PlaySoundMem(TitleBGM, DX_PLAYTYPE_BACK);
	}

	if (g_KeyFlg & PAD_INPUT_DOWN)if (++MenuNo > 3)MenuNo = 0;
	if (g_KeyFlg & PAD_INPUT_UP)if (--MenuNo < 0)MenuNo = 3;

	// Z�L�[�Ń��j���[�I��
	if (g_KeyFlg & PAD_INPUT_A)g_GameState = MenuNo + 1;

	DrawGraph(0, 0, g_TitleImage, FALSE);
	DrawGraph(120, 200, g_Menu, TRUE);
	DrawRotaGraph(90, 220 + MenuNo * 40, 0.7f, M_PI / 2, g_Cone, TRUE);

}

void GameInit(void) {
	g_Score = 0;
	StartFlg = true;
	timer = TIMELIMIT;
	for (int i = 0; i < 4; i++) {
		apple_count[i] = 0;
	}

	AppleFunc.AppleInit();
	
	//spflag = 0;		//�`�������W5�p
	//bikec = 0;		//�`�������W5�p

	////	���s������������
	//g_Mileage = 0;

	////	�G1����������̏����ݒ�
	//g_EnemyCount1 = 0;
	//g_EnemyCount2 = 0;
	//g_EnemyCount3 = 0;
	//g_EnemyCount4 = 0;

	//	�v���C���[�̏����ݒ�
	g_player.flg = TRUE;
	g_player.x = PLAYER_POS_X;
	g_player.y = PLAYER_POS_Y;
	g_player.w = PLAYER_WIDTH;
	g_player.h = PLAYER_HEIGHT;
	//g_player.angle = 0.0;
	g_player.count = 0;
	g_player.speed = PLAYER_SPEED;
	//g_player.hp = PLAYER_HP;
	//g_player.fuel = PLAYER_FUEL;
	//g_player.bari = PLAYER_BARRIER;
	//g_player.bariup = PLAYER_BARRIERUP;
	//apple[0].AppleInit();

	g_GameState = 5;

	//for (int i = 0; i < ENEMY_MAX; i++) {
	//	apple_flg = apple[i].GetAppleFlg();
	//	apple_flg = false;
	//	//apple_x = apple[i].GetAppleX();
	//	

	//	
	//}
	//for (int i = 0; i < ENEMY_MAX; i++) {	//�`�������W5
	//	g_enemy2[i].flg = FALSE;
	//}
	//for (int i = 0; i < ITEM_MAX; i++) {
	//	g_item[i].flg = FALSE;
	//}
}

void DrawRanking(void) {
	//	�X�y�[�X�L�[�Ń��j���[�ɖ߂�
	if (g_KeyFlg & PAD_INPUT_M)g_GameState = 0;

	DrawGraph(0, 0, g_RankingImage, FALSE);


	SetFontSize(18);
	for (int i = 0; i < RANKING_DATA; i++) {
		DrawFormatString(50, 170 + i * 25, 0xffffff, "%2d  %-10s  %10d", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
		DrawString(100, 450, "---- �X�y�[�X�L�[�������ă^�C�g���֖߂� ----", 0xffffff, 0);
	}
}

void DrawHelp(void) {
	if (g_KeyFlg & PAD_INPUT_M)	g_GameState = 0;

	DrawGraph(0, 0, g_TitleImage, FALSE);
	SetFontSize(16);
	DrawString(20, 120, "�w���v���", 0xffffff, 0);

	DrawString(20, 160, "����͏�Q��������Ȃ���", 0xffffff, 0);
	DrawString(20, 180, "���葱����Q�[���ł�", 0xffffff, 0);
	//DrawString(20, 200, "�R�����s���邩��Q����", 0xffffff, 0);
	//DrawString(20, 220, "���񓖂���ƃQ�[���I�[�o�[�ł�", 0xffffff, 0);
	//DrawString(20, 250, "�A�C�e���ꗗ", 0xffffff, 0);
	//DrawGraph(20, 260, g_Item[0], TRUE);
	//DrawString(20, 315, "���ƔR�����񕜂����B", 0xffffff, 0);
	//DrawGraph(20, 335, g_Item[1], TRUE);
	//DrawString(20, 385, "�_���[�W���󂯂Ă��鎞�Ɏ��Ƒϋv��", 0xffffff, 0);
	//DrawString(20, 405, "�ϋv�������Ă��Ȃ�������R���������񕜂����B", 0xffffff, 0);
	DrawString(20, 450, "---- �X�y�[�X�L�[�������ă^�C�g���֖߂� ----", 0xffffff, 0);
}

void DrawEnd(void) {
	int g_PosY = 0;
	//�G���h�摜�\��
	DrawGraph(0, 0, g_EndImage, FALSE);

	//�G���f�B���O�\��
	if (++g_WaitTime < 600) g_PosY = 300 - g_WaitTime / 2;

	SetFontSize(24);
	DrawString(100, 170 + g_PosY, "�^�C�g���@�@�@��񂲂̂���", 0xFFFFFF,0);
	DrawString(100, 200 + g_PosY, "�o�[�W�����@�@1.0", 0xFFFFFF,0);
	DrawString(100, 230 + g_PosY, "�ŏI�X�V���@�@2022�N6��28��", 0xFFFFFF,0);
	DrawString(100, 260 + g_PosY, "����ҁ@�@�@�@���,�䂤��", 0xFFFFFF,0);
	DrawString(100, 290 + g_PosY, "�@�@�@�@�@�@�@���傤��,����", 0xFFFFFF,0);
	DrawString(100, 310 + g_PosY, "�f�ޗ��p", 0xFFFFFF);
	DrawString(100, 340 + g_PosY, "�@BGM�@�@,�@ DOVA-SYNDROME", 0xFFFFFF,0);
	DrawString(100, 365 + g_PosY, "�@SE�@�@�@�@�@���ʉ��H�[", 0xFFFFFF,0);

	//�^�C���̉��Z�������I��
	if (++g_WaitTime > 900) g_GameState = 99;
}

void GameMain(void) {
	//if (g_KeyFlg & PAD_INPUT_M)g_GameState = 6;	//�e�X�g�p�̏����I�H

	//SetFontSize(16);
	//DrawString(20, 20, "�Q�[�����C��", 0xffffff, 0);
	//DrawString(150, 450, "---- �X�y�[�X�L�[�������ăQ�[���I�[�o�[�� ----", 0xffffff, 0);

	/*BackScrool();*/

	/*EnemyControl();*/
	//g_enemy2->h = 100;	//	�`�������W5
	/*BikeControl();

	ItemControl();*/

	if (timer-- == 0) {
		g_GameState = 6;
	}

	DrawGraph(0, 0, g_StageImage, FALSE);
	AppleFunc.AppleControl();

	PlayerControl();
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


	//g_Mileage = g_Score;

	DrawBox(150, 150, 490, 330, 0x009900, TRUE);
	DrawBox(150, 150, 490, 330, 0x000000, FALSE);

	SetFontSize(20);
	DrawString(220, 170, "�Q�[���I�[�o�[", 0xcc0000);
	SetFontSize(16);
	DrawString(180, 200, "���s����     ", 0x000000);
	DrawRotaGraph(230, 230, 0.3f, M_PI / 2, apple_img[0], TRUE, FALSE);
	DrawRotaGraph(230, 250, 0.3f, M_PI / 2, apple_img[1], TRUE, FALSE);
	DrawRotaGraph(230, 270, 0.3f, M_PI / 2, apple_img[2], TRUE, FALSE);
	DrawRotaGraph(230, 290, 0.3f, M_PI / 2, apple_img[3], TRUE, FALSE);
	//DrawRotaGraph(230, 230, 0.3f, M_PI / 2, apple_img[0], TRUE, FALSE);
	//DrawRotaGraph(230, 250, 0.3f, M_PI / 2, apple_img[1], TRUE, FALSE);
	//DrawRotaGraph(230, 270, 0.3f, M_PI / 2, apple_img[2], TRUE, FALSE);
	//DrawRotaGraph(230, 290, 0.3f, M_PI / 2, apple_img[3], TRUE, FALSE);



	//DrawFormatString(260, 200, 0xFFFFFF, " %6d x  10 = %6d", g_Mileage / 10, g_Mileage / 10 * 10);
	//DrawFormatString(260, 285, 0xFFFFFF, " %6d x 300 = %6d", g_EnemyCount4, g_EnemyCount4 * 300);
	//DrawFormatString(260, 222, 0xFFFFFF, " %6d x  50 = %6d", g_EnemyCount3, g_EnemyCount3 * 50);
	//DrawFormatString(260, 243, 0xFFFFFF, "%6d x  100 = %6d", g_EnemyCount2, g_EnemyCount2 * 100);
	//DrawFormatString(260, 264, 0xFFFFFF, "%6d x  200 = %6d", g_EnemyCount1, g_EnemyCount1 * 200);

	DrawString(280, 310, "�X�R�A ", 0x000000);
	DrawFormatString(310, 310, 0xFFFFFF, "         = %6d", g_Score);
	DrawString(150, 450, "---- �X�y�[�X�L�[�������ă^�C�g���֖߂� ----", 0xffffff, 0);

}



int LoadImages() {
	if (LoadDivGraph("images/apple.png", 5, 4, 1, 50, 50, apple_img) == -1) return -1;

	if ((g_TitleImage = LoadGraph("images/Title.png")) == -1)return-1;
	if ((g_Menu = LoadGraph("images/menu.bmp")) == -1)return-1;
	if ((g_Cone = LoadGraph("images/cone.bmp")) == -1)return-1;
	//if ((g_Item[0] = LoadGraph("images/greenapple.png")) == -1)return-1;
	//if ((g_Item[1] = LoadGraph("images/apple.png")) == -1)return-1;
	if ((g_EndImage = LoadGraph("images/background.png")) == -1)return-1;
	if (LoadDivGraph("images/apple.png", 4, 4, 1, 50, 50, apple_img) == -1)return -1;	//�����S
	if ((g_StageImage = LoadGraph("images/pause.png")) == -1)return-1;
	if (LoadDivGraph("images/player.png", 6, 3, 2, 32, 32, players_img) == -1)return -1;	//�����S
	return 0;
}

void InputRanking(void)
{
	//�����L���O�摜�\��
	DrawGraph(0, 0, g_RankingImage, FALSE);

	// �t�H���g�T�C�Y�̐ݒ�
	SetFontSize(20);

	// ���O���͎w��������̕`��
	DrawString(150, 240, "�����L���O�ɓo�^���܂�", 0xFFFFFF);
	DrawString(150, 270, "���O���p���œ��͂��Ă�������", 0xFFFFFF);

	// ���O�̓���
	DrawString(150, 310, "> ", 0xFFFFFF);
	DrawBox(160, 305, 300, 335, 0x000055, TRUE);
	if (KeyInputSingleCharString(170, 310, 10, g_Ranking[RANKING_DATA - 1].name, FALSE) == 1) {
		g_Ranking[RANKING_DATA - 1].score = g_Score;	// �����L���O�f�[�^�̂P�O�ԖڂɃX�R�A��o�^
		SortRanking();		// �����L���O���בւ�
		SaveRanking();		// �����L���O�f�[�^�̕ۑ�
		g_GameState = 2;		// �Q�[�����[�h�̕ύX
	}

}

void SortRanking(void)
{
	int i, j;
	RankingData work;

	// �I��@�\�[�g
	for (i = 0; i < RANKING_DATA - 1; i++) {
		for (j = i + 1; j < RANKING_DATA; j++) {
			if (g_Ranking[i].score <= g_Ranking[j].score) {
				work = g_Ranking[i];
				g_Ranking[i] = g_Ranking[j];
				g_Ranking[j] = work;
			}
		}
	}

	// ���ʕt��
	for (i = 0; i < RANKING_DATA; i++) {
		g_Ranking[i].no = 1;
	}
	// ���_�������ꍇ�́A�������ʂƂ���
	// �����ʂ��������ꍇ�̎��̏��ʂ̓f�[�^�������Z���ꂽ���ʂƂ���
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

	// �t�@�C���I�[�v��
	if ((fp = fopen("dat/05/rankingdata.txt", "w")) == NULL) {
		/* �G���[���� */
		printf("Ranking Data Error\n");
		return -1;
	}

	// �����L���O�f�[�^���z��f�[�^����������
	for (int i = 0; i < RANKING_DATA; i++) {
		fprintf(fp, "%2d %10s %10d\n", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
	}

	//�t�@�C���N���[�Y
	fclose(fp);

	return 0;

}

int ReadRanking(void)
{
	FILE* fp;
#pragma warning(disable:4996)

	//�t�@�C���I�[�v��
	if ((fp = fopen("dat/05/rankingdata.txt", "r")) == NULL) {
		//�G���[����
		printf("Ranking Data Error\n");
		return -1;
	}

	//�����L���O�f�[�^�z����f�[�^��ǂݍ���
	for (int i = 0; i < RANKING_DATA; i++) {

		fscanf(fp, "%2d %10s %10d", &g_Ranking[i].no, g_Ranking[i].name, &g_Ranking[i].score);
	}

	//�t�@�C���N���[�Y
	fclose(fp);

	return 0;
}

void PlayerControl() {

	//	�㉺���E�ړ�
	if (g_player.flg == TRUE) {
		if (g_NowKey & PAD_INPUT_LEFT)	g_player.x -= g_player.speed;
		if (g_NowKey & PAD_INPUT_RIGHT)	g_player.x += g_player.speed;

	}

	//	��ʂ��͂ݏo���Ȃ��悤�ɂ���
	if (g_player.x < 32)		g_player.x = 32;

	if (g_player.x > SCREEN_WIDTH - 160)		g_player.x = SCREEN_WIDTH - 160;

	//	�v���C���[�̕\��
	if (g_player.flg == TRUE) {
		if (g_NowKey & PAD_INPUT_LEFT) {
			DrawRotaGraph(g_player.x, g_player.y, 2.3f, -M_PI / 18, players_img[0], TRUE, FALSE);
		}
		else if (g_NowKey & PAD_INPUT_RIGHT) {
			DrawRotaGraph(g_player.x, g_player.y, 2.3f, -M_PI / 18, players_img[0], TRUE, FALSE);
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

	//	�G�����������\��
	DrawBox(500, 0, 640, 480, 0x009900, TRUE);
	SetFontSize(16);
	//DrawFormatString(510, 20, 0x000000, "SCORE:%d",g_Score);
	DrawFormatString(540, 20, 0x000000, "�c�莞��");
	DrawFormatString(560, 60, 0x000000, "%d", timer/60);
	//DrawFormatString(560, 40, 0xFFFFFF, "%08d", g_Ranking[0].score);
	DrawFormatString(540, 160, 0x000000, "�̂�����");
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

	//	x,y �͒��S���W�Ƃ���
	int sx1 = p->x - (p->w / 2);
	int sy1 = p->y - (p->h / 2);
	int sx2 = sx1 + p->w;
	int sy2 = sy1 + p->h;

	int dx1 = e->GetAppleX() - ((e->GetAppleW() / 2) - 20);
	int dy1 = e->GetAppleY() - (e->GetAppleH() / 2);
	int dx2 = dx1 + e->GetAppleW() - 50;
	int dy2 = dy1 + e->GetAppleH();

	//	�Z�`���d�Ȃ��Ă���Γ�����
	if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
		return TRUE;
	}
	return FALSE;
}

int LoadSounds(void)
{
	//�^�C�g��BGM
	if ((TitleBGM = LoadSoundMem("Sound/BGM/�ق�.wav")) == -1) return -1;
	if ((GameMainBGM = LoadSoundMem("Sound/BGM/�~�j�}���ȃ}�[�`.wav")) == -1) return -1;
	if ((RankingBGM = LoadSoundMem("Sound/BGM/Walking_Ameba.wav")) == -1) return -1;
	if ((EndBGM = LoadSoundMem("Sound/BGM/Small_Happy.wav")) == -1) return -1;
	if ((SE = LoadSoundMem("Sound/SE/select.wav")) == -1) return -1;

}