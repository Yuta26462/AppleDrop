#include "main.h"

struct RankingData g_Ranking[RANKING_DATA];

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

		fscanf(fp, "%2d %10s %5d", &g_Ranking[i].no, g_Ranking[i].name, &g_Ranking[i].score);
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