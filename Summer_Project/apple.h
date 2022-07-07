#pragma once

#define APPLE_MAX 10			//�����S�̍ő�`�搔
#define APPLE_START 4			//�����S�̏����`�搔
#define RED_SCORE 150			//�ԃ����S�̃X�R�A
#define GREEN_SCORE 300			//�΃����S�̃X�R�A
#define GOLD_SCORE 500			//�������S�̃X�R�A
#define BLACK_SCORE -1000		//�������S�̃X�R�A

#define RED_SPEED 2				//�ԃ����S�̗������x
#define GREEN_SPEED 5			//�΃����S�̗������x
#define GOLD_SPEED 10			//�������S�̗������x
#define BLACK_SPEED 1			//�������S�̗������x


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
	void AppleControl(bool pauseflg);			//�����S���������֐�
	int CreateApple(int maxapple);	//�����S�쐬�֐�
	void AppleInit();				//�����S�������֐�

	int GetApplePos(int apple_speed,int num);
	void CheckAppleSpeed(int speed1, int speed2,bool* over_flg);
	bool* GetAppleFlg();			//�����Sflg�擾�֐�
	int GetAppleX();				//�����Sx���W�擾�֐�
	int GetAppleY();				//�����Sy���W�擾�֐�
	int GetAppleW();				//�����S���擾�֐�
	int GetAppleH();				//�����S�����擾�֐�
};

int GetAppleType();
int GetAppleSpeed(int AppleType);




