#pragma once

#define APPLE_MAX 10			//リンゴの最大描画数
#define APPLE_START 4			//リンゴの初期描画数
#define RED_SCORE 150			//赤リンゴのスコア
#define GREEN_SCORE 300			//緑リンゴのスコア
#define GOLD_SCORE 500			//金リンゴのスコア
#define BLACK_SCORE -1000		//黒リンゴのスコア

#define RED_SPEED 2				//赤リンゴの落下速度
#define GREEN_SPEED 5			//緑リンゴの落下速度
#define GOLD_SPEED 10			//金リンゴの落下速度
#define BLACK_SPEED 1			//黒リンゴの落下速度


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
	void AppleControl();			//リンゴ落下処理関数
	void CreateApple(int maxapple);	//リンゴ作成関数
	void AppleInit();				//リンゴ初期化関数

	int GetApplePos(int apple_speed,int num,int *made_apples);
	int CheckAppleSpeed(int speed1, int speed2);
	bool GetAppleFlg();			//リンゴflg取得関数
	int GetAppleX();				//リンゴx座標取得関数
	int GetAppleY();				//リンゴy座標取得関数
	int GetAppleW();				//リンゴ幅取得関数
	int GetAppleH();				//リンゴ高さ取得関数
	int GetAppleCount(int type);
	//int getpos();					//デバッグ用
	//int getspeed();
};


int GetAppleType();
int GetAppleSpeed(int AppleType);

extern bool Over_flg;			//リンゴが重なっているかのフラグ
extern int apple_quantity;			//描画されているリンゴの個数


