#pragma once
#include "DxLib.h"
#define _USE_MATH_DEFINES
#include <math.h>
#define RANKING_DATA 5

class Menu {
public:
	void menu(void);
	void GameModeChange(int num);
	int LoadImages(void);
};

extern Menu menu;


int g_OldKey, g_NowKey, g_KeyFlg;
int g_GameState = 0;
int g_TitleImage;
int g_Menu, g_Cone;

int g_Score = 0, g_RankingImage;

int LoadImages();

int g_Item[2];

int g_WaitTime = 0;
int g_EndImage;

int g_Mileage;	//走行距離y
int g_EnemyCount1, g_EnemyCount2, g_EnemyCount3;
int g_EnemyCount4; //	チャレンジ5
int g_Teki[3];	//キャラ画像変数

int g_StageImage;
int g_Car, g_Barrier;

int spflag;		//チャレンジ4用フラグ
int g_Bike[1];		//チャレンジ5	バイク画像変数
int bikec = 0;

int g_PosY, gPosX;

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

void PlayerControl();

void EnemyControl();
void BikeControl();		//チャレンジ5用
int CreateEnemy();
int CreateBike();		//チャレンジ5用

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int PLAYER_POS_X = SCREEN_WIDTH / 2;
const int PLAYER_POS_Y = SCREEN_HEIGHT - 100;
const int PLAYER_WIDTH = 63;
const int PLAYER_HEIGHT = 120;
const int PLAYER_SPEED = 5;
const int PLAYER_HP = 1000;
const int PLAYER_FUEL = 20000;
const int PLAYER_BARRIER = 3;
const int PLAYER_BARRIERUP = 10;
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
struct ENEMY g_enemy00 = { TRUE,0,0,0,-50,63,120,0,1 };
struct ENEMY g_enemyCn = { TRUE,4,0,0,-50,18,18,0,1 };


struct  ENEMY g_enemy2[ENEMY_MAX];//	チャレンジ5
struct ENEMY g_item[ITEM_MAX];
struct ENEMY g_item00 = { TRUE,0,0,0,-50,50,50,0,1 };

struct	RankingData {
	int no;
	char name[11];
	long score;
};
struct RankingData		g_Ranking[RANKING_DATA];

int HitBoxPlayer(PLAYER* p, ENEMY* e);
void ItemControl();		//	アイテム処理
int CreateItem();		//	アイテム生成処理