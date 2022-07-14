#pragma once
#include "DxLib.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "apple.h"
#include "ranking.h"
#include "player.h"

#define Select_X								1
#define Select_Y								2
#define AnalogInput_X								1
#define AnalogInput_Y								2

#define INPUT_A									1
#define INPUT_B									2
#define INPUT_X									3
#define INPUT_Y									4
#define INPUT_BACK								7
#define INPUT_START								8

enum GAME_STATUS{ DRAW_GAMETITLE, GAME_INIT, DRAW_RANKING, DRAW_HELP, DRAW_END, GAME_MAIN, INPUT_RANKING, END = 99 };
enum BGM{BGM_Title = 1, BGM_GameMain,BGM_Ranking, BGM_END, SE_Selecter, SE_OK, SE_Return, SE_Key_Remove, SE_GoldenApple,SE_Red_AND_Green_Apple,SE_PoisonApple};
enum IMAGE{Image_Title, Image_Stage, Image_Ranking, Image_End};
enum PLAYER_IMAGE{Image_LeftPlayer, Image_IDOL_LeftPlayer, Image_DASH_LeftPlayer, Image_DASH_RightPlayer, Image_IDOL_RightPlayer, Image_RightPlayer, Image_TOP_Player, Image_TOP_IDOLPlayer, Image_TOP_Player2};
enum APPLE_IMAGE{Image_RedApple, Image_GreenApple, Image_GoldenApple, Image_PoisonApple};
enum FONT{Font_Menu = 1, Font_Pause};

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
extern PLAYER player;


extern bool StartFlg;
extern bool AllReset;


extern int g_Score;

extern Apple apple[APPLE_MAX];
extern Apple AppleFunc;


//int g_Item[2];


//int g_Mileage;	//走行距離y
//int g_EnemyCount1, g_EnemyCount2, g_EnemyCount3;
//int g_EnemyCount4; //	チャレンジ5






//int spflag;		//チャレンジ4用フラグ
//int g_Bike[1];		//チャレンジ5	バイク画像変数
//int bikec = 0;


int GetTimeLimit(void);
void GameInit(void);
void GameMain(void);

void DrawGameTitle(void);
void DrawEnd(void);
void DrawHelp(void);
int GetGameStatus(void);
void SetGameStatus(int GameStatus);

int SetTimer(int num);
int GetTimer(void);
void ResetTimer(void);
int GetAnalogInput(int xy);
bool PadInput(int Key);
int GetSound(int sound_name);
int GetImage(int imagename);
int GetFont(int num);
int GetSelect(int xy);
bool isPause(void);
int GetPlayerImage(int player_status);
int GetAppleImage(int type);

//void BackScrool();

void PlayerControl(bool pauseflg);


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;






void ItemControl();		//	アイテム処理
int CreateItem();		//	アイテム生成処理