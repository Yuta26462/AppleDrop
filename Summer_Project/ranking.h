#pragma once
#include <string>	//ranking�Ŏg�p

#define RANKING_DATA			5
#define KEYBORA_X				100
#define KEYBORA_Y				260
#define KEYBORA_XMARGIN			30
#define KEYBORA_YMARGIN			30
#define Selecter_BOXSIZE		30

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