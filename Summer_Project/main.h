#pragma once
#include "DxLib.h"
#include "main.h"
#include "apple.h"
#include "ranking.h"																														
#include "player.h"
#define _USE_MATH_DEFINES
#include <math.h>

class Menu {
public:
	void menu(void);
	void GameModeChange(int num);
	int LoadImages(void);
};




void GameInit(void);
void GameMain(void);

void DrawGameTitle(void);
void DrawGameOver(void);
void DrawEnd(void);
void DrawHelp(void);

void DrawRanking(void);
void InputRanking(void);

void SortRanking(void);
int SaveRanking(void);
int ReadRanking(void);

void BackScrool();

void EnemyControl();
void BikeControl();		//チャレンジ5用
int CreateEnemy();
int CreateBike();		//チャレンジ5用

const int ENEMY_MAX = 8;
const int ITEM_MAX = 3;


struct PLAYER {
	int flg;
	int x, y;
	int w, h;
	double angle;
	int count;
	int speed;
	int hp;
	int fuel;
	int bari;
	int baricnt;
	int bariup;
};
struct PLAYER g_player;

struct ENEMY {
	int flg;
	int type;
	int img;
	int x, y, w, h;
	int speed;
	int point;
};
struct  ENEMY g_enemy[ENEMY_MAX];
struct  ENEMY g_enemy2[ENEMY_MAX];//	チャレンジ5
struct ENEMY g_item[ITEM_MAX];

int HitBoxPlayer(PLAYER* p, ENEMY* e);
void ItemControl();		//	アイテム処理
int CreateItem();		//	アイテム生成処理