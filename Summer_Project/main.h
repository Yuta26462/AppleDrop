#pragma once
#include "DxLib.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "apple.h"
#include "ranking.h"

#define XINPUT_A								16
#define XINPUT_B								32
#define XINPUT_X								64
#define XINPUT_Y								128
#define XINPUT_START							2048
#define XINPUT_BACK								1024
#define DINPUT_A								32
#define DINPUT_B								64
#define DINPUT_X								16
#define DINPUT_Y								128
#define DINPUT_START							8192
#define DINPUT_BACK								4096

#define INPUT_A									1
#define INPUT_B									2
#define INPUT_X									3
#define INPUT_Y									4
#define INPUT_BACK								7
#define INPUT_START								8


enum {BGM_Title = 1, BGM_GameMain,BGM_Ranking, BGM_END, SE_Selecter, SE_OK, SE_Return, SE_Key_Remove, SE_GoldenApple,SE_Red_AND_Green_Apple,SE_PoisonApple};
enum {Image_Title, Image_Stage, Image_Ranking, Image_End };

//サウンド用変数
extern int GoldenApple_SE, Red_AND_Green_Apple_SE, PoisonApple_SE;

class Menu {
public:
	void menu(void);
	void GameModeChange(int num);
	int LoadImages(void);
};

extern Menu menu;
extern Ranking ranking;


extern int g_OldKey, g_NowKey, g_KeyFlg;
extern int g_GameState;
extern bool StartFlg;
extern int timer;


extern int g_Score;
extern int g_RankingImage;
extern int invincibletime;			//無敵判定時間

extern Apple apple[APPLE_MAX];
extern Apple AppleFunc;
extern bool apple_flg;
extern int apple_count[4];
extern int apple_img[4];	//キャラ画像変数


//int g_Item[2];


//int g_Mileage;	//走行距離y
//int g_EnemyCount1, g_EnemyCount2, g_EnemyCount3;
//int g_EnemyCount4; //	チャレンジ5






//int spflag;		//チャレンジ4用フラグ
//int g_Bike[1];		//チャレンジ5	バイク画像変数
//int bikec = 0;



void GameInit(void);
void GameMain(void);

void DrawGameTitle(void);
void DrawEnd(void);
void DrawHelp(void);

void DrawRanking(void);
void InputRanking(void);
int SetTimer(int num);
int GetTimer(void);
void ResetTimer(void);
bool PadInput(int Key);
int GetSound(int sound_name);
int GetImage(int imagename);

//void BackScrool();

void PlayerControl(bool pauseflg);


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int PLAYER_POS_X = SCREEN_WIDTH / 2;
const int PLAYER_POS_Y = SCREEN_HEIGHT - 50;
const int PLAYER_WIDTH = 58;
const int PLAYER_HEIGHT = 62;
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
	bool Poisonflg;
};
extern struct PLAYER g_player;

//struct ENEMY {
//	int flg;
//	int type;
//	int img;
//	int x, y, w, h;
//	int speed;
//	int point;
//};
//struct  ENEMY g_enemy[ENEMY_MAX];
//struct ENEMY g_enemy00 = { TRUE,0,0,0,-50,63,120,0,1 };
//struct ENEMY g_enemyCn = { TRUE,4,0,0,-50,18,18,0,1 };
//
//
//struct  ENEMY g_enemy2[ENEMY_MAX];//	チャレンジ5
//struct ENEMY g_item[ITEM_MAX];
//struct ENEMY g_item00 = { TRUE,0,0,0,-50,50,50,0,1 };


int HitBoxPlayer(PLAYER* p, Apple* e);
void ItemControl();		//	アイテム処理
int CreateItem();		//	アイテム生成処理