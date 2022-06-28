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