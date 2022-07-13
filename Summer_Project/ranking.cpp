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

	//ファイルオープン
	if ((fp = fopen("dat/rankingdata.txt", "r")) == NULL) {
		//エラー処理
		printf("Ranking Data Error\n");
		return -1;
	}

	//ランキングデータ配分列データを読み込む
	for (int i = 0; i < RANKING_DATA; i++) {

		fscanf(fp, "%2d %10s %5d", &g_Ranking[i].no, g_Ranking[i].name, &g_Ranking[i].score);
	}

	//ファイルクローズ
	fclose(fp);

	return 0;
}


int  Ranking::SaveRanking(void)
{
	FILE* fp;
#pragma warning(disable:4996)

	// ファイルオープン
	if ((fp = fopen("dat/rankingdata.txt", "w")) == NULL) {
		/* エラー処理 */
		printf("Ranking Data Error\n");
		return -1;
	}

	// ランキングデータ分配列データを書き込む
	for (int i = 0; i < RANKING_DATA; i++) {
		fprintf(fp, "%2d %10s %5d\n", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
	}

	//ファイルクローズ
	fclose(fp);

	return 0;

}


void Ranking::SortRanking(void)
{
	int i, j;
	RankingData work;

	// 選択法ソート
	for (i = 0; i < RANKING_DATA - 1; i++) {
		for (j = i + 1; j < RANKING_DATA; j++) {
			if (g_Ranking[i].score <= g_Ranking[j].score) {
				work = g_Ranking[i];
				g_Ranking[i] = g_Ranking[j];
				g_Ranking[j] = work;
			}
		}
	}

	// 順位付け
	for (i = 0; i < RANKING_DATA; i++) {
		g_Ranking[i].no = 1;
	}
	// 得点が同じ場合は、同じ順位とする
	// 同順位があった場合の次の順位はデータ個数が加算された順位とする
	for (i = 0; i < RANKING_DATA - 1; i++) {
		for (j = i + 1; j < RANKING_DATA; j++) {
			if (g_Ranking[i].score > g_Ranking[j].score) {
				g_Ranking[j].no++;
			}
		}
	}
}