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
void DrawEnd(void);						//�G���h��ʂ�`�悷��B
void DrawHelp(void);					//�ւ�Չ�ʂ��`�悷��B
int GetGameStatus(void);				//�Q�[����ʂ��擾����B
void SetGameStatus(int GameStatus);		//�Q�[����ʂ��Z�b�g����B

int SetTimer(int num);					//�^�C�}�[���Z�b�g����( num = 0 �őO�u++�ŃZ�b�g)
int GetTimer(void);						//�^�C�}�[���擾����B
void ResetTimer(void);					//�^�C�}�[�����Z�b�g����B
int GetAnalogInput(int xy);				//�A�i���O�X�e�B�b�N�̓���XY���擾����B
bool PadInput(int Key);					//�R���g���[���[�̓��͏�Ԃ��m�F����B
int GetSound(int sound_name);			//�����f�[�^���擾����B
int GetImage(int imagename);			//�摜�f�[�^���擾����B
int GetFont(int num);					//�t�H���g�f�[�^���擾����B
int GetSelect(int xy);					//�A�i���O�X�e�B�b�N�̐ÓI����XY���擾����B
bool isPause(void);						//�|�[�Y��Ԃ��擾����B
int GetPlayerImage(int player_status);	//�v���C���[�摜���擾����B
int GetAppleImage(int type);			//��񂲂̎�ޕʉ摜���擾����B
int GetMainScore(void);					//�Q�[�����C���̃X�R�A���擾����B
void SetMainScore(int score);			//�Q�[�����C���̃X�R�A���Z�b�g����B
bool GetStartFlg(void);					//
void SetStartFlg(bool Flg);				//
bool GetAllReset(void);					//
void SetAllReset(bool Flg);				//

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;