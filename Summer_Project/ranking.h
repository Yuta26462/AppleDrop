#pragma once
#define RANKING_DATA 5
#include <string>	//ranking‚ÅŽg—p
extern struct RankingData g_Ranking[RANKING_DATA];

struct	RankingData {
	int no;
	char name[11];
	long score;
};

class Ranking {
public:
	int GetRankingNum(int num);
	char* GetRankingName(int num);
	long GetRankingScore(int num);
	int ReadRanking(void);
	int  SaveRanking(void);
	void SortRanking(void);
	void DrawRanking(void);
	void InputRanking(void);
};