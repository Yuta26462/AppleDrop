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
	void PlayerControl(bool pauseflg);
	void ResetPlayer(void);
	int GetPlayerTransition(int xyhy);
	void SetStatus(int status);
	int GetStatus(void);
	int GetPlayerFlg(void);
	int HitBoxPlayer(Apple* e);
};
