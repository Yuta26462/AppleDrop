#pragma once
#define RANKING_DATA 5

struct	RankingData {
	int no;
	char name[11];
	long score;
};
struct RankingData		g_Ranking[RANKING_DATA];

class Ranking {
private:
	FILE* fp;
	RankingData data[RANKING_DATA];

public:
	void InputRanking(void);	
	void SortRanking(void);
	int  SaveRanking(void);
	int ReadRanking(void);
	void DrawRanking(void);
	int GetScore(int n);
};

Ranking ranking;																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																										