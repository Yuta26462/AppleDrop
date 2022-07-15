#pragma once
#include "apple.h"

#define PlayerX 1
#define PlayerY 2
#define PlayerH 3
#define PlayerW 4
#define	Poison_ON	1
#define	Poison_OFF	2

class PLAYER {
	int flg;
	int x, y;
	int w, h;
	double angle;
	int count;
	int speed;
	bool Poisonflg;
	int PadSpeedTimer;
	int player_angle;
public:
	void PlayerControl();				//プレイヤー操作
	void ResetPlayer(void);				//プレイヤー初期化処理
	int GetPlayerTransition(int xyhw);	//プレイヤーの(X,Y,H,W)座標を取得する。
	void SetStatus(int status);			//プレイヤーの状態効果を設定する。
	int GetStatus(void);				//プレイヤーの状態効果を取得する。
	int GetPlayerFlg(void);				//プレイヤーの有効性を取得する。
	int HitBoxPlayer(Apple* e);			//りんごとプレイヤーの当たり判定を計算する。
};
