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
	void PlayerControl();				//�v���C���[����
	void ResetPlayer(void);				//�v���C���[����������
	int GetPlayerTransition(int xyhw);	//�v���C���[��(X,Y,H,W)���W���擾����B
	void SetStatus(int status);			//�v���C���[�̏�Ԍ��ʂ�ݒ肷��B
	int GetStatus(void);				//�v���C���[�̏�Ԍ��ʂ��擾����B
	int GetPlayerFlg(void);				//�v���C���[�̗L�������擾����B
	int HitBoxPlayer(Apple* e);			//��񂲂ƃv���C���[�̓����蔻����v�Z����B
};
