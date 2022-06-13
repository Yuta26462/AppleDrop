#pragma once
#include <cstdio>
#include <iostream>
#include <DxLib.h>

class Menu {
public:
	void menu(void);
	void GameModeChange(int num);
};

extern Menu menu;