#pragma once
#define RANKING_DATA 5
#include <string>	//rankingで使用
extern struct RankingData g_Ranking[RANKING_DATA];

struct	RankingData {
	int no;
	char name[11];
	long score;
};

class Ranking {
public:
	int GetRankingNum(int num);			//ランキングのNo.を取得する。
	char* GetRankingName(int num);		//ランキングの名前を取得する。
	long GetRankingScore(int num);		//ランキングのスコアを取得する。
	int ReadRanking(void);				//ランキングを読み込み。
	int  SaveRanking(void);				//ランキングの保存。
	void SortRanking(void);				//ランキングのソート(入れ替え)。
	void DrawRanking(void);				//ランキングの表示
	void InputRanking(void);			//ランキングデータ登録。
};