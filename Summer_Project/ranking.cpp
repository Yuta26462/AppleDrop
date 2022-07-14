#include "main.h"

struct RankingData g_Ranking[RANKING_DATA];

void Ranking::DrawRanking(void) {

	if (CheckSoundMem(GetSound(BGM_Title)) == 1)StopSoundMem(GetSound(BGM_Title));
	if (CheckSoundMem(GetSound(BGM_GameMain)) == 1)StopSoundMem(GetSound(BGM_GameMain));
	if (CheckSoundMem(GetSound(BGM_Ranking)) == 0)PlaySoundMem(GetSound(BGM_Ranking), DX_PLAYTYPE_BACK);

	if (PadInput(INPUT_B)) { PlaySoundMem(GetSound(SE_Return), DX_PLAYTYPE_BACK); SetGameStatus(DRAW_GAMETITLE); }

	DrawGraph(0, 0, GetImage(Image_Ranking), FALSE);

	for (int i = 0; i < RANKING_DATA; i++) {
		DrawFormatStringToHandle(50, 120 + i * 50, 0xffffff, GetFont(Font_Menu), "%2d  %-10s", g_Ranking[i].no, g_Ranking[i].name);
		DrawFormatStringToHandle(380, 120 + i * 50, 0xffffff, GetFont(Font_Menu), "%5d", g_Ranking[i].score);

		if (SetTimer(0) < 120) {
			DrawString(150, 390, "---- B�{�^���[�������ă^�C�g���ւ��ǂ� ----", 0xffffff, 0);
		}
		else if (GetTimer() > 360) { ResetTimer(); }
	}
}

void Ranking::InputRanking(void)
{
	if (CheckSoundMem(GetSound(BGM_GameMain)) == 1)StopSoundMem(GetSound(BGM_GameMain));
	if (CheckSoundMem(GetSound(BGM_Ranking)) == 0)PlaySoundMem(GetSound(BGM_Ranking), DX_PLAYTYPE_BACK);
	//�����L���O�摜�\��
	DrawGraph(0, 0, GetImage(Image_Ranking), FALSE);

	static char default_char[10] = "_________";
	static char buf[10] = "_________";
	static int input_i = 0;
	static int errorflg = 0;

	// ���O���͎w��������̕`��
	DrawFormatStringToHandle(120, 100, 0xFFFFFF, GetFont(Font_Menu), "�����L���O�ɓo�^���܂�");
	DrawFormatStringToHandle(65, 150, 0xFFFFFF, GetFont(Font_Menu), "���O���p���œ��͂��Ă�������");
	if (GetTimer() < 30) {
		DrawString(180, 420, "---- START�{�^���������Ė��O����I ----", 0xffffff, 0);
	}
	else if (GetTimer() > 60) { ResetTimer(); }
	// ���O�̓���
	DrawFormatStringToHandle(120, 200, 0xFFFFFF, GetFont(Font_Menu), "> ");
	DrawBox(190, 200, 425, 250, 0x000055, TRUE);

	if (errorflg == 1 && (60 > SetTimer(0))) {
		DrawFormatStringToHandle(120, 220, 0xff1493, GetFont(Font_Menu), "���O����͂��Ă�������");
	}
	else if (GetTimer() > 60) { ResetTimer(); errorflg = 0; }

	for (int i = 0; input_i > i; i++) { DrawFormatStringToHandle(195 + i * 25, 205, 0xFFFFFF, GetFont(Font_Menu), "%c", buf[i]); }
	/*if(display > input_i){ DrawFormatStringToHandle(200 + input_i * 25, 205, 0xFFFFFF, MenuFont, "%c", buf[input_i - 1]); }*/

	if (SetTimer(0) < 30) {
		//DrawFormatStringToHandle(200, 205, 0xFFFFFF, MenuFont, "%s", buf);
		for (int display = 10; input_i < display; display--) {
			DrawFormatStringToHandle(170 + display * 25, 205, 0xFFFFFF, GetFont(Font_Menu), "%c", buf[display - 1]);
		}
	}
	else if (GetTimer() > 60) { ResetTimer(); }

	char input_char = '0';
	for (int j = 0; j < 5; j++) {
		for (int i = 1; i < 14; i++) {
			if (input_char < '{' || input_char < '[') {
				DrawFormatString(i * 30 + 100, 260 + 30 * j, 0x000000, "%c", input_char++);
			}
			if (input_char == ':') { input_char = 'a'; j = 1; i = 0; }
			if (input_char == '{') { input_char = 'A'; j = 3; i = 0; }
		}
	}
	static int selecterX = 0;
	static int selecterY = 0;

	//�J�[�\���̈ʒu�����Z�b�g
	if (AllReset) {
		selecterX = 0;
		selecterY = 0;
		AllReset = false;
	}

	if (GetSelect(Select_X) == 1)if (++selecterX > 12)selecterX = 0; else if (selecterY == 0 && selecterX > 9)selecterX = 0;
	if (GetSelect(Select_X) == -1)if (--selecterX < 0)selecterX = 12; if (selecterY == 0 && selecterX > 9)selecterX = 9;
	if (GetSelect(Select_Y) == 1)if (++selecterY > 4)selecterY = 0;
	if (GetSelect(Select_Y) == -1)if (--selecterY < 0)selecterY = 4;
	if (PadInput(INPUT_A)) {}
	if (GetSelect(Select_X) == -1 || GetSelect(Select_X) == 1 || GetSelect(Select_Y) == -1 || GetSelect(Select_Y) == 1) { PlaySoundMem(GetSound(SE_Selecter), DX_PLAYTYPE_BACK); }
	DrawBox(120 + 30 * selecterX, 250 + 30 * selecterY, 150 + 30 * selecterX, 280 + 30 * selecterY, 0x696969, FALSE);


	if (PadInput(INPUT_START)) {
		PlaySoundMem(GetSound(SE_OK), DX_PLAYTYPE_BACK);
		if (input_i <= 0) { errorflg = 1; }
		else {
			buf[input_i] = '\0';
			std::string buf_str = buf;
			buf_str = buf_str.erase(input_i);
			/*strcpyDx(ranking.GetRankingName(RANKING_DATA - 1), buf_str.c_str());*/
			strcpyDx(g_Ranking[RANKING_DATA - 1].name, buf_str.c_str());

			g_Ranking[RANKING_DATA - 1].score = g_Score;	// �����L���O�f�[�^�̂P�O�ԖڂɃX�R�A��o�^
			ranking.SortRanking();		//�����L���O���בւ�
			ranking.SaveRanking();		//�����L���O�f�[�^�̕ۑ�
			input_i = 0;
			strcpyDx(buf, default_char);
			SetGameStatus(DRAW_RANKING);
		}
	}
	if (PadInput(INPUT_A) && input_i < 9) {
		PlaySoundMem(GetSound(SE_OK), DX_PLAYTYPE_BACK);
		if (selecterY == 0 && selecterX >= 0 && selecterX <= 9) { buf[input_i++] = (char)48 + selecterX; }
		if (selecterY == 1 && selecterX >= 0 && selecterX <= 12) { buf[input_i++] = (char)97 + selecterX; }
		if (selecterY == 2 && selecterX >= 0 && selecterX <= 12) { buf[input_i++] = (char)110 + selecterX; }
		if (selecterY == 3 && selecterX >= 0 && selecterX <= 12) { buf[input_i++] = (char)65 + selecterX; }
		if (selecterY == 4 && selecterX >= 0 && selecterX <= 12) { buf[input_i++] = (char)78 + selecterX; }
		if (!isalnum(buf[input_i - 1])) { buf[--input_i] = '_'; }
	}
	if (PadInput(INPUT_B)) {
		PlaySoundMem(GetSound(SE_Key_Remove), DX_PLAYTYPE_BACK);
		if (input_i > 0)buf[--input_i] = '_';
	}
}

int Ranking::GetRankingNum(int num) {
	if (g_Ranking[num].no < 6) {
		int no = g_Ranking[num].no;
		return no;
	}
	return 0;
}

char* Ranking::GetRankingName(int num) {
	bool checkflg = false;
	if (0 <= num && num <= 6) {
		for (int i = 0; i < 9; i++) {
			if (isalnum(g_Ranking[num].name[i]) != 0)checkflg = true;
		}
		if (checkflg == true) {
			char* name = g_Ranking[num].name;
			return name;
		}
	}
	return 0;
}

long Ranking::GetRankingScore(int num) {
	long score = 0;;
	if (0 <= num && num <= 6) {
		if ((-99999 >= g_Ranking[num].score && g_Ranking[num].score <= 99999) && (0 <= num && num <= 6))
			score = g_Ranking[num].score;
		return score;
	}
	return 0;
}

int Ranking::ReadRanking(void)
{
	FILE* fp;
#pragma warning(disable:4996)

	//�t�@�C���I�[�v��
	if ((fp = fopen("dat/rankingdata.txt", "r")) == NULL) {
		//�G���[����
		printf("Ranking Data Error\n");
		return -1;
	}

	//�����L���O�f�[�^�z����f�[�^��ǂݍ���
	for (int i = 0; i < RANKING_DATA; i++) {

		fscanf_s(fp, "%2d %10s %5d", &g_Ranking[i].no, &g_Ranking[i].name, 9, &g_Ranking[i].score);
	}

	//�t�@�C���N���[�Y
	fclose(fp);

	return 0;
}


int  Ranking::SaveRanking(void)
{
	FILE* fp;
#pragma warning(disable:4996)

	// �t�@�C���I�[�v��
	if ((fp = fopen("dat/rankingdata.txt", "w")) == NULL) {
		/* �G���[���� */
		printf("Ranking Data Error\n");
		return -1;
	}

	// �����L���O�f�[�^���z��f�[�^����������
	for (int i = 0; i < RANKING_DATA; i++) {
		fprintf(fp, "%2d %10s %5d\n", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
	}

	//�t�@�C���N���[�Y
	fclose(fp);

	return 0;

}


void Ranking::SortRanking(void)
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