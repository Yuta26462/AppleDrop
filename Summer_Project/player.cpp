#include "player.h"
#include "main.h"


void PLAYER::PlayerControl(bool pauseflg) {
	static int checkflg = 0;
	static int player_angle = 0;
	static int old_player_angle = 0;
	if (!isPause()) {
		if (flg == TRUE) {
			int i = 0;
			PadSpeedTimer++;
			if (PadSpeedTimer > 20 - i) {
				PadSpeedTimer = 0;
				if (speed < 6 && speed > -6) {
					/*++speed; i += 4;*/ old_player_angle = player_angle; checkflg = 0;
					if (GetAnalogInput(AnalogInput_X) < -0) { speed++; /*i += JoyPadX / 200;*/ }
					if (GetAnalogInput(AnalogInput_X) > 0) { speed++; /*i += JoyPadSX / 200;*/ }
				}
				if (GetAnalogInput(AnalogInput_X) >= -100 && GetAnalogInput(AnalogInput_X) <= 100) {
					i = 0;
					if (speed > 0)speed -= 2;
					if (speed < 0)speed++;
				}
				if (old_player_angle != player_angle && speed < 6) {
					if (GetAnalogInput(AnalogInput_X) < -100 && player_angle == -1) {
						speed--;
					}
				}
			}


			if (old_player_angle != player_angle && speed > 3) {
				speed = -2;
			}
			if (player_angle == 1) { x += speed; }
			if (player_angle == -1) { x -= speed; }
		}

		//	画面をはみ出さないようにする
		if (x < 32) { x = 32; speed = 0; }

		if (x > SCREEN_WIDTH - 170) { x = SCREEN_WIDTH - 170; speed = 0; }


		if (Poisonflg == TRUE && GetTimer() >= 120) {
			Poisonflg = false;
			ResetTimer();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

	}
	if (isPause()) {
		if (player_angle == -1)DrawRotaGraph(x, y, 2.3f, 0, GetPlayerImage(Image_DASH_LeftPlayer), TRUE, FALSE);
		if (player_angle == 1)DrawRotaGraph(x, y, 2.3f, 0, GetPlayerImage(Image_DASH_RightPlayer), TRUE, FALSE);
	}
	else if ((Poisonflg == TRUE && SetTimer(0) % 40 < 20) || Poisonflg == FALSE) {
		if (flg == TRUE) {
			if(player_angle == 0){ DrawRotaGraph(x, y, 2.3f, 0, GetPlayerImage(Image_TOP_IDOLPlayer), TRUE, FALSE);}
			if (GetAnalogInput(AnalogInput_X) < -0 || player_angle == -1) {
				DrawRotaGraph(x, y, 2.3f, -M_PI / 18, GetPlayerImage(Image_LeftPlayer), TRUE, FALSE); player_angle = -1;
			}
			if (GetAnalogInput(AnalogInput_X) > 0 || player_angle == 1) {
				DrawRotaGraph(x, y, 2.3f, -M_PI / 18, GetPlayerImage(Image_RightPlayer), TRUE, FALSE); player_angle = 1;
			}
			if (GetAnalogInput(AnalogInput_X) == 0)
			{
				if (player_angle == -1)DrawRotaGraph(x, y, 2.3f, 0, GetPlayerImage(Image_IDOL_LeftPlayer), TRUE, FALSE);
				if (player_angle == 1)DrawRotaGraph(x, y, 2.3f, 0, GetPlayerImage(Image_IDOL_RightPlayer), TRUE, FALSE);
			}
			if (speed > 3) {
				if (player_angle == -1)DrawRotaGraph(x, y, 2.3f, 0, GetPlayerImage(Image_DASH_LeftPlayer), TRUE, FALSE);
				if (player_angle == 1)DrawRotaGraph(x, y, 2.3f, 0, GetPlayerImage(Image_DASH_RightPlayer), TRUE, FALSE);
			}
		}
		else {
			DrawRotaGraph(x, y, 0.3f, M_PI / 8 * (++count / 5), GetPlayerImage(Image_LeftPlayer), TRUE, FALSE);
			if (count >= 80)		flg = TRUE;
		}
		
	}
}

void PLAYER::ResetPlayer(void) {
	//	プレイヤーの初期設定
	flg = TRUE;
	x = PLAYER_POS_X;
	y = PLAYER_POS_Y;
	w = PLAYER_WIDTH;
	h = PLAYER_HEIGHT;
	count = 0;
	speed = 0;
	Poisonflg = false;
}

int PLAYER::GetPlayerTransition(int xyhy) {
	if (xyhy == PlayerX)return x;
	if (xyhy == PlayerY)return y;
	if (xyhy == PlayerH)return h;
	if (xyhy == PlayerW)return y;
	return 0;
}

void PLAYER::SetStatus(int status) {
	if (status == Poison_ON) { Poisonflg = true; }
	if (status == Poison_OFF) { Poisonflg = false; }
}

int PLAYER::GetStatus(void) {
	if (Poisonflg == true) { return Poison_ON; }
	if (Poisonflg == false) { return Poison_OFF; }
	return 0;
}

int PLAYER::GetPlayerFlg(void) {
	if (FALSE | TRUE)return flg;
	return -1;
}

int PLAYER::HitBoxPlayer(Apple* e) {

	//	x,y は中心座標とする
	int sx1 = x - (w / 2);
	int sy1 = y - (h / 2);
	int sx2 = sx1 + w;
	int sy2 = sy1 + h;

	int dx1 = e->GetAppleX() - ((e->GetAppleW() / 2));
	int dy1 = e->GetAppleY() - (e->GetAppleH() / 2);
	int dx2 = dx1 + e->GetAppleW();
	int dy2 = dy1 + e->GetAppleH();

	//	短形が重なっていれば当たり
	if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
		return TRUE;
	}
	return FALSE;
}