#include "player.h"
#include "main.h"


void PLAYER_CLASS::PlayerControl(bool pauseflg) {
	static int checkflg = 0;
	static int player_angle = 0;
	static int old_player_angle = 0;
	if (!isPause()) {
		if (g_player.flg == TRUE) {
			int i = 0;
			PadSpeedTimer++;
			if (PadSpeedTimer > 20 - i) {
				PadSpeedTimer = 0;
				if (g_player.speed < 6 && g_player.speed > -6) {
					/*++g_player.speed; i += 4;*/ old_player_angle = player_angle; checkflg = 0;
					if (GetAnalogInput(AnalogInput_X) < -0) { g_player.speed++; /*i += JoyPadX / 200;*/ }
					if (GetAnalogInput(AnalogInput_X) > 0) { g_player.speed++; /*i += JoyPadSX / 200;*/ }
				}
				if (GetAnalogInput(AnalogInput_X) >= -100 && GetAnalogInput(AnalogInput_X) <= 100) {
					i = 0;
					if (g_player.speed > 0)g_player.speed -= 2;
					if (g_player.speed < 0)g_player.speed++;
				}
				if (old_player_angle != player_angle && g_player.speed < 6) {
					if (GetAnalogInput(AnalogInput_X) < -100 && player_angle == -1) {
						g_player.speed--;
					}
				}
			}


			if (old_player_angle != player_angle && g_player.speed > 3) {
				g_player.speed = -2;
			}
			if (player_angle == 1) { g_player.x += g_player.speed; }
			if (player_angle == -1) { g_player.x -= g_player.speed; }
		}

		//	âÊñ ÇÇÕÇ›èoÇ≥Ç»Ç¢ÇÊÇ§Ç…Ç∑ÇÈ
		if (g_player.x < 32) { g_player.x = 32; g_player.speed = 0; }

		if (g_player.x > SCREEN_WIDTH - 170) { g_player.x = SCREEN_WIDTH - 170; g_player.speed = 0; }


		if (g_player.Poisonflg == TRUE && GetTimer() >= 120) {
			g_player.Poisonflg = false;
			ResetTimer();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

	}
	if (isPause()) {
		if (player_angle == -1)DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, GetPlayerImage(Image_DASH_LeftPlayer), TRUE, FALSE);
		if (player_angle == 1)DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, GetPlayerImage(Image_DASH_RightPlayer), TRUE, FALSE);
	}
	else if ((g_player.Poisonflg == TRUE && SetTimer(0) % 40 < 20) || g_player.Poisonflg == FALSE) {
		if (g_player.flg == TRUE) {
			if(player_angle == 0){ DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, GetPlayerImage(Image_TOP_IDOLPlayer), TRUE, FALSE);}
			if (GetAnalogInput(AnalogInput_X) < -0 || player_angle == -1) {
				DrawRotaGraph(g_player.x, g_player.y, 2.3f, -M_PI / 18, GetPlayerImage(Image_LeftPlayer), TRUE, FALSE); player_angle = -1;
			}
			if (GetAnalogInput(AnalogInput_X) > 0 || player_angle == 1) {
				DrawRotaGraph(g_player.x, g_player.y, 2.3f, -M_PI / 18, GetPlayerImage(Image_RightPlayer), TRUE, FALSE); player_angle = 1;
			}
			if (GetAnalogInput(AnalogInput_X) == 0)
			{
				if (player_angle == -1)DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, GetPlayerImage(Image_IDOL_LeftPlayer), TRUE, FALSE);
				if (player_angle == 1)DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, GetPlayerImage(Image_IDOL_RightPlayer), TRUE, FALSE);
			}
			if (g_player.speed > 3) {
				if (player_angle == -1)DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, GetPlayerImage(Image_DASH_LeftPlayer), TRUE, FALSE);
				if (player_angle == 1)DrawRotaGraph(g_player.x, g_player.y, 2.3f, 0, GetPlayerImage(Image_DASH_RightPlayer), TRUE, FALSE);
			}
		}
		else {
			DrawRotaGraph(g_player.x, g_player.y, 0.3f, M_PI / 8 * (++g_player.count / 5), GetPlayerImage(Image_LeftPlayer), TRUE, FALSE);
			if (g_player.count >= 80)		g_player.flg = TRUE;
		}
		
	}
}