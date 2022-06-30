#pragma once

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

class Player {
	int g_OldKey, g_NowKey, g_KeyFlg;
public:
	void PlayerControl();
	int KeyInput(void);
};

Player player;