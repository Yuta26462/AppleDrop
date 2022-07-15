#pragma once
#define RANKING_DATA 5
#include <string>	//ranking�Ŏg�p
extern struct RankingData g_Ranking[RANKING_DATA];

struct	RankingData {
	int no;
	char name[11];
	long score;
};

class Ranking {
public:
	int GetRankingNum(int num);			//�����L���O��No.���擾����B
	char* GetRankingName(int num);		//�����L���O�̖��O���擾����B
	long GetRankingScore(int num);		//�����L���O�̃X�R�A���擾����B
	int ReadRanking(void);				//�����L���O��ǂݍ��݁B
	int  SaveRanking(void);				//�����L���O�̕ۑ��B
	void SortRanking(void);				//�����L���O�̃\�[�g(����ւ�)�B
	void DrawRanking(void);				//�����L���O�̕\��
	void InputRanking(void);			//�����L���O�f�[�^�o�^�B
};