#pragma once

#define APPLE_MAX 10
#define RED_SCORE 150
#define GREEN_SCORE 300
#define GOLD_SCORE 500
#define BLACK_SCORE 1000

#define RED_SPEED 2
#define GREEN_SPEED 5
#define GOLD_SPEED 10
#define BLACK_SPEED 1

enum APPLES
{
	RED_APPLE,
	GREEN_APPLE,
	GOLD_APPLE,
	BLACK_APPLE
};

class Apple {
private:
	bool flg;
	int type;
	int img;
	int x, y, w, h;
	int speed;
	int score;
public:
	void AppleControl();
	int CreateApple();
	
	

	bool* GetAppleFlg();
	int GetAppleX();
	int GetAppleY();
	int GetAppleW();
	int GetAppleH();
};

int GetAppleType();
int GetAppleSpeed(int AppleType);
int GetAppleImage(int AppleType);

extern Apple apple[APPLE_MAX];
extern bool apple_flg;

