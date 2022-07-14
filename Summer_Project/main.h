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


extern Ranking ranking;
extern PLAYER player;


extern Apple apple[APPLE_MAX];


int GetTimeLimit(void);
void GameInit(void);
void GameMain(void);

void DrawGameTitle(void);
void DrawEnd(void);						//エンド画面を描画する。
void DrawHelp(void);					//へるぷ画面んを描画する。
int GetGameStatus(void);				//ゲーム画面を取得する。
void SetGameStatus(int GameStatus);		//ゲーム画面をセットする。

int SetTimer(int num);					//タイマーをセットする( num = 0 で前置++でセット)
int GetTimer(void);						//タイマーを取得する。
void ResetTimer(void);					//タイマーをリセットする。
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

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;