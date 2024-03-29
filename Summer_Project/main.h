#pragma once
#include "DxLib.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "apple.h"
#include "ranking.h"
#include "player.h"
#define	VERSION									(float)2.47
#define LAST_UPDATED							"2022年7月20日"

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
enum BGM_SE { BGM_Title = 1, BGM_GameMain, BGM_Ranking, BGM_END, SE_Selecter, SE_OK, SE_Return, SE_Key_Remove, SE_GoldenApple, SE_Red_AND_Green_Apple, SE_PoisonApple, SE_Count, SE_Whistle };
enum IMAGE{Image_Title, Image_Stage, Image_Ranking,Image_RankingInside, Image_End};
enum PLAYER_IMAGE{Image_LeftPlayer, Image_IDOL_LeftPlayer, Image_DASH_LeftPlayer, Image_DASH_RightPlayer, Image_IDOL_RightPlayer, Image_RightPlayer, Image_TOP_Player, Image_TOP_IDOLPlayer, Image_TOP_Player2};
enum APPLE_IMAGE{Image_RedApple, Image_GreenApple, Image_GoldenApple, Image_PoisonApple};
enum FONT{Font_Title = 1, Font_Menu, Font_Pause,Font_Key};


extern Ranking ranking;
extern PLAYER player;


extern Apple apple[11];


int GetTimeLimit(void);
void GameInit(void);
void GameMain(void);

void DrawGameTitle(void);
void DrawEnd(void);						//エンド画面を描画する。
void DrawHelp(void);					//へるぷ画面んを描画する。
int GetGameStatus(void);				//ゲーム画面を取得する。
void SetGameStatus(int GameStatus);		//ゲーム画面をセットする。

int Timer(int time, int index = 0);		//タイマーをセット・ゲット・リセットする(time = (1:Set, 0:Get. -1:Reset))
int GetAnalogInput(int xy);				//アナログスティックの入力XYを取得する。
bool PadInput(int Key);					//コントローラーの入力状態を確認する。
int GetSound(int sound_name);			//音声データを取得する。
int GetImage(int imagename);			//画像データを取得する。
int GetFont(int num);					//フォントデータを取得する。
int GetSelect(int xy);					//アナログスティックの静的入力XYを取得する。
bool isPause(void);						//ポーズ状態を取得する。
int GetPlayerImage(int player_status);	//プレイヤー画像を取得する。
int GetAppleImage(int type);			//りんごの種類別画像を取得する。
int GetMainScore(void);					//ゲームメインのスコアを取得する。
void SetMainScore(int score);			//ゲームメインのスコアをセットする。
bool GetStartFlg(void);					//
void SetStartFlg(bool Flg);				//
bool GetAllReset(void);					//
void SetAllReset(bool Flg);				//
int NewTimer(int time);					//
void DrawFinish(void);					//ゲーム終了時のアニメーション

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;