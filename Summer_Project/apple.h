#pragma once

#define APPLE_MAX 10			//ƒŠƒ“ƒS‚ÌÅ‘å•`‰æ”
#define APPLE_START 4			//ƒŠƒ“ƒS‚Ì‰Šú•`‰æ”
#define RED_SCORE 150			//ÔƒŠƒ“ƒS‚ÌƒXƒRƒA
#define GREEN_SCORE 300			//—ÎƒŠƒ“ƒS‚ÌƒXƒRƒA
#define GOLD_SCORE 500			//‹àƒŠƒ“ƒS‚ÌƒXƒRƒA
#define BLACK_SCORE -1000		//•ƒŠƒ“ƒS‚ÌƒXƒRƒA

#define RED_SPEED 2				//ÔƒŠƒ“ƒS‚Ì—‰º‘¬“x
#define GREEN_SPEED 5			//—ÎƒŠƒ“ƒS‚Ì—‰º‘¬“x
#define GOLD_SPEED 10			//‹àƒŠƒ“ƒS‚Ì—‰º‘¬“x
#define BLACK_SPEED 1			//•ƒŠƒ“ƒS‚Ì—‰º‘¬“x


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
	int pos;
	int x, y, w, h;
	int speed;
	int score;
public:
	void AppleControl(bool pauseflg);			//ƒŠƒ“ƒS—‰ºˆ—ŠÖ”
	int CreateApple(int maxapple);	//ƒŠƒ“ƒSì¬ŠÖ”
	void AppleInit();				//ƒŠƒ“ƒS‰Šú‰»ŠÖ”

	int GetApplePos(int apple_speed,int num);
	void CheckAppleSpeed(int speed1, int speed2,bool* over_flg);
	bool* GetAppleFlg();			//ƒŠƒ“ƒSflgæ“¾ŠÖ”
	int GetAppleX();				//ƒŠƒ“ƒSxÀ•Wæ“¾ŠÖ”
	int GetAppleY();				//ƒŠƒ“ƒSyÀ•Wæ“¾ŠÖ”
	int GetAppleW();				//ƒŠƒ“ƒS•æ“¾ŠÖ”
	int GetAppleH();				//ƒŠƒ“ƒS‚‚³æ“¾ŠÖ”
};

int GetAppleType();
int GetAppleSpeed(int AppleType);




