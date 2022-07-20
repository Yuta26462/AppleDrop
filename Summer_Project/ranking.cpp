#include "main.h"

struct RankingData g_Ranking[RANKING_DATA];

void Ranking::DrawRanking(void) {

	if (CheckSoundMem(GetSound(BGM_Title)) == 1)StopSoundMem(GetSound(BGM_Title));
	if (CheckSoundMem(GetSound(BGM_GameMain)) == 1)StopSoundMem(GetSound(BGM_GameMain));
	if (CheckSoundMem(GetSound(BGM_Ranking)) == 0)PlaySoundMem(GetSound(BGM_Ranking), DX_PLAYTYPE_BACK);

	if (PadInput(INPUT_B)) { PlaySoundMem(GetSound(SE_Return), DX_PLAYTYPE_BACK); SetGameStatus(DRAW_GAMETITLE); }

	//DrawGraph(0, 0, GetImage(Image_Ranking), FALSE);
	DrawRotaGraph(320, 260, 0.7f, 0, GetImage(Image_Ranking), FALSE);
	DrawExtendGraph(55, 110, 585, 400, GetImage(Image_RankingInside), FALSE);
	DrawStringToHandle(170, 40, "ランキング", 0xFFFF33, GetFont(Font_Title),0xFFFFFF);
	//DrawBoxAA(55, 110, 585, 400, 0x74F634, TRUE, 0.0F);	//外枠背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBoxAA(55, 110, 585, 400, 0xbdff3d, FALSE, 5.0F);	//外枠
	DrawLineAA(55, 160, 585, 160, 0xbdff3d, 5.0F); //横見出し列
	DrawLineAA(110, 110, 110, 400, 0xbdff3d,2.0F);	//1列目
	DrawLineAA(450, 110, 450, 400, 0xbdff3d, 2.0F);	//3列目
	for(int i= 1; i < 5; i++){DrawLineAA(55, 155 + i * 50, 585, 155 + i * 50, 0xbdff3d, 2.0F);}//中間線

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawStringToHandle(58, 113, "No", 0xFFFFFF, GetFont(Font_Menu), 0x000000);
	DrawStringToHandle(240, 113, "名前", 0xFFFFFF, GetFont(Font_Menu), 0x000000);
	DrawStringToHandle(455, 113, "スコア", 0xFFFFFF, GetFont(Font_Menu), 0x000000);
	for (int i = 0; i < RANKING_DATA; i++) {
		static int color = 0xFFFFFF;
		if (i == 0) { color = 0xfffd3d; }
		else if(i == 1){ color = 0xffbc3d; }
		else if (i == 2) { color = 0xc9c9c9; }
		else { color = 0xFFFFFF; }

		DrawFormatStringToHandle(60, 160 + i * 50, color, GetFont(Font_Menu), "%2d", g_Ranking[i].no);
		DrawFormatStringToHandle(120, 160 + i * 50, color, GetFont(Font_Menu), "%9s", g_Ranking[i].name);
		DrawFormatStringToHandle(455, 160 + i * 50, color, GetFont(Font_Menu), "%5d", g_Ranking[i].score);

		if (Timer(1) < 120) {
			DrawString(150, 410, "---- Bボタンーをおしてタイトルへもどる ----", 0xffffff, 0);
		}
		else if (Timer(0) > 360) { Timer(-1); }
	}
}

void Ranking::InputRanking(void)
{
	if (CheckSoundMem(GetSound(BGM_GameMain)) == 1)StopSoundMem(GetSound(BGM_GameMain));
	if (CheckSoundMem(GetSound(BGM_Ranking)) == 0)PlaySoundMem(GetSound(BGM_Ranking), DX_PLAYTYPE_BACK);
	//ランキング画像表示
	DrawGraph(0, 0, GetImage(Image_Ranking), FALSE);

	static char default_char[10] = "_________";			//初期表示
	static char buf[10] = "_________";					//入力値
	static int input_i = 0;								//入力値(1文字)
	static int errorflg = 0;							//エラー検出用フラグ
	static char KeyMap_Qwerty[3][12] = { "QWERTYUIOP","ASDFGHJKL","ZXCVBNM" };
	
	// 名前入力指示文字列の描画
	DrawFormatStringToHandle(120, 60, 0xFFFFFF, GetFont(Font_Menu), "ランキングに登録します");
	DrawFormatStringToHandle(65, 110, 0xFFFFFF, GetFont(Font_Menu), "名前を英字で入力してください");
	if (Timer(0) < 30) {
		DrawString(180, 420, "---- STARTボタンをおして名前決定！ ----", 0xffffff, 0);
	}
	else if (Timer(0) > 60) { Timer(-1); }
	// 名前の入力
	DrawFormatStringToHandle(120, 200, 0xFFFFFF, GetFont(Font_Menu), "> ");
	DrawBox(190, 200, 425, 250, 0x000055, TRUE);

	if (errorflg == 1 && (60 > Timer(1))) {
		DrawFormatStringToHandle(120, 220, 0xff1493, GetFont(Font_Menu), "名前を入力してください");
	}
	else if (Timer(0) > 60) { Timer(-1); errorflg = 0; }

	for (int i = 0; input_i > i; i++) { DrawFormatStringToHandle(195 + i * 25, 205, 0xFFFFFF, GetFont(Font_Menu), "%c", buf[i]);
	//DrawFormatString(195 + i * 40, 180, 0x000000, "%d", GetDrawStringWidthToHandle(&buf[i], strlen(&buf[i]), GetFont(Font_Menu)));
	}
	/*if(display > input_i){ DrawFormatStringToHandle(200 + input_i * 25, 205, 0xFFFFFF, MenuFont, "%c", buf[input_i - 1]); }*/
	if (Timer(1) < 30) {
		//DrawFormatStringToHandle(200, 205, 0xFFFFFF, MenuFont, "%s", buf);
		for (int display = 10; input_i < display; display--) {
			DrawFormatStringToHandle(170 + display * 25, 205, 0xFFFFFF, GetFont(Font_Menu), "%c", buf[display - 1]);
		}
	}
	else if (Timer(0) > 60) { Timer(-1); }

	static int KeyBoard_X = KEYBORA_X;
	static int KeyBoard_XMrgin = KEYBORA_XMARGIN;
	static int KeyMap = 0;
	static int Uplow = 0;
	if (PadInput(INPUT_Y) && KeyMap == 1) {
		PlaySoundMem(GetSound(SE_OK), DX_PLAYTYPE_BACK);
		if (Uplow == 0) { Uplow = 1; } else if (Uplow == 1)Uplow = 0; }

	char input_char = '0';
	if (KeyMap == 0) {
		for (int j = 0; j < 5; j++) {
			for (int i = 1; i < 14; i++) {
				if (input_char < '{' || input_char < '[') {
					DrawFormatString(i * KeyBoard_XMrgin + KeyBoard_X, KEYBORA_Y + KEYBORA_YMARGIN * j, 0x000000, "%c", input_char++);
				}
				if (input_char == ':') { input_char = 'a'; j = 1; i = 0; }
				if (input_char == '{') { input_char = 'A'; j = 3; i = 0; }
			}
		}
	}
	if (KeyMap == 1) {

		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		//DrawBox(160, 260, 460, 380, 0XFFD700,TRUE);
		//DrawBox(160, 260, 460, 380, 0x808000, FALSE);
		//DrawFillBox(160, 230, 450, 420, 0x000000);
		//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawCircle(214, 397, 10, 0x000000, 1);
		DrawString(210, 390, "Y", 0xffff00);
		DrawString(234, 390, "大文字／小文字切り替え",0xFFFFFF);
		int i_space = 0;
		for (int j = 0; j < 4; j++) {
			for (int i = 1; i < 14; i++) {
				if (input_char < '{' || input_char < '[') {
					if (j == 0)DrawFormatString(i * KEYBORA_XMARGIN + KeyBoard_X + i_space, KEYBORA_Y + KEYBORA_YMARGIN * j, 0x000000, "%c", input_char++);
					if (j > 0) {
						input_char = KeyMap_Qwerty[0][0];
						DrawFormatString(i * KEYBORA_XMARGIN + KeyBoard_X + i_space, KEYBORA_Y + KEYBORA_YMARGIN * j, 0x000000, "%c", KeyMap_Qwerty[j - 1][i - 1] + (Uplow == 1 ? 0 : (char)32));
						if (j == 1 && i == 10) { i_space = 15; break; }
						if (j == 2 && i == 9) { i_space = 30; break; }
						if (j == 3 && i == 7)break;
					}
				}
				if (input_char == ':') {
					j = 1; i = 0;
				}
			}
		}
	}
	static int selecterX = 0;
	static int selecterY = 0;
	static int color = 0x696969;

	//カーソルの位置をリセット
	if (GetAllReset()) {
		selecterX = 0;
		selecterY = 0;
		SetAllReset(false);
	}


	if (KeyMap == 0) {
		KeyBoard_X = KEYBORA_X;
		KeyBoard_XMrgin = KEYBORA_XMARGIN;
		if (GetSelect(Select_X) == 1)if (++selecterX > 12)selecterX = 0; else if (selecterY == 0 && selecterX > 9)selecterX = 0;
		if (GetSelect(Select_X) == -1)if (--selecterX < 0)selecterX = 12; 
		if (GetSelect(Select_Y) == 1)if (++selecterY > 4)selecterY = 0;  else if (selecterY == 2 && selecterX > 9)selecterX = 0;
		if (GetSelect(Select_Y) == -1)if (--selecterY < 0)selecterY = 4;
		if (selecterY == 0 && selecterX > 9)selecterX = 9;
	}
	else if (KeyMap == 1) {
		KeyBoard_X = KEYBORA_X + 40;
		if (GetSelect(Select_X) == 1)if (++selecterX > 9) { selecterX = 0; }
		else if (selecterY == 2 && selecterX > 8){selecterX = 0; }
		else if (selecterY == 3 && selecterX > 6){selecterX = 0;}
		if (GetSelect(Select_X) == -1)if (--selecterX < 0)selecterX = 9;
		if (GetSelect(Select_Y) == 1) if (++selecterY > 3)selecterY = 0;
		if (GetSelect(Select_Y) == -1)if (--selecterY < 0)selecterY = 3;
		if (selecterY == 2)KeyBoard_XMrgin = KEYBORA_XMARGIN + (KEYBORA_XMARGIN / 2); else if (selecterY == 3)KeyBoard_XMrgin = KEYBORA_XMARGIN * 2; else KeyBoard_XMrgin = KEYBORA_XMARGIN;
		
		if (selecterY == 0 && selecterX > 9)selecterX = 0;
		if (selecterY == 1 && selecterX > 9)selecterX = 9;
		if (selecterY == 2 && selecterX > 8)selecterX = 8;
		if (selecterY == 3 && selecterX > 6)selecterX = 6;
		if (selecterY > 3)selecterY = 3;	//切り替え時見失い防止
	}
	//文字入力時の色変更
	if (Timer(1, 1) && PadInput(INPUT_A) && input_i < 9) { Timer(-1, 1); color = 0xffffff; }
	else if (Timer(0, 1) > 15) { color = 0x000000; Timer(-1, 1); }

	if (GetSelect(Select_X) == -1 || GetSelect(Select_X) == 1 || GetSelect(Select_Y) == -1 || GetSelect(Select_Y) == 1) { PlaySoundMem(GetSound(SE_Selecter), DX_PLAYTYPE_BACK); }
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox((KeyBoard_X + KeyBoard_XMrgin - 10) + Selecter_BOXSIZE * selecterX, (KEYBORA_Y  - 10) + Selecter_BOXSIZE * selecterY, (KeyBoard_X + KeyBoard_XMrgin + 20) + Selecter_BOXSIZE * selecterX, (KEYBORA_Y + 20) + Selecter_BOXSIZE * selecterY, color, FALSE);
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	if (PadInput(INPUT_X)) {
		PlaySoundMem(GetSound(SE_OK), DX_PLAYTYPE_BACK);
		if (KeyMap == 0) { KeyMap = 1;  }
		else if (KeyMap == 1)KeyMap = 0;
	}

	if (PadInput(INPUT_START)) {
		
		if (input_i <= 0) { errorflg = 1; PlaySoundMem(GetSound(SE_Key_Remove), DX_PLAYTYPE_BACK); StartJoypadVibration(DX_INPUT_PAD1, 600, 150, -1);
		}
		else {
			PlaySoundMem(GetSound(SE_OK), DX_PLAYTYPE_BACK);
			StartJoypadVibration(DX_INPUT_PAD1, 600, 150, -1);
			buf[input_i] = '\0';
			std::string buf_str = buf;
			buf_str = buf_str.erase(input_i);
			/*strcpyDx(ranking.GetRankingName(RANKING_DATA - 1), buf_str.c_str());*/
			strcpyDx(g_Ranking[RANKING_DATA - 1].name, buf_str.c_str());

			g_Ranking[RANKING_DATA - 1].score = GetMainScore();	// ランキングデータの１０番目にスコアを登録
			ranking.SortRanking();		//ランキング並べ替え
			ranking.SaveRanking();		//ランキングデータの保存
			input_i = 0;
			strcpyDx(buf, default_char);
			SetAllReset(true);
			SetGameStatus(DRAW_RANKING);
			//名前確定確定画面へ移行
		}
	}
	if (PadInput(INPUT_A) && input_i < 9) {
		PlaySoundMem(GetSound(SE_OK), DX_PLAYTYPE_BACK);
		if (KeyMap == 0) {
			if (selecterY == 0 && selecterX >= 0 && selecterX <= 9) { buf[input_i++] = (char)48 + selecterX; }
			if (selecterY == 1 && selecterX >= 0 && selecterX <= 12) { buf[input_i++] = (char)97 + selecterX; }
			if (selecterY == 2 && selecterX >= 0 && selecterX <= 12) { buf[input_i++] = (char)110 + selecterX; }
			if (selecterY == 3 && selecterX >= 0 && selecterX <= 12) { buf[input_i++] = (char)65 + selecterX; }
			if (selecterY == 4 && selecterX >= 0 && selecterX <= 12) { buf[input_i++] = (char)78 + selecterX; }
		}

		else if (KeyMap == 1) {
			if (selecterY == 0 && selecterX >= 0 && selecterX <= 10) { buf[input_i++] = (char)48 + selecterX; }
			if (selecterY == 1 && selecterX >= 0 && selecterX <= 9) { buf[input_i++] = KeyMap_Qwerty[selecterY - 1][selecterX] + (Uplow == 1 ? 0 : (char)32); }
			if (selecterY == 2 && selecterX >= 0 && selecterX <= 9) { buf[input_i++] = KeyMap_Qwerty[selecterY - 1][selecterX] + (Uplow == 1 ? 0 : (char)32); }
			if (selecterY == 3 && selecterX >= 0 && selecterX <= 7) { buf[input_i++] = KeyMap_Qwerty[selecterY - 1][selecterX] + (Uplow == 1 ? 0 : (char)32); }
		}
		if (input_i > 0 && !isalnum(buf[input_i - 1])) { buf[--input_i] = '_'; }
	}
	if (PadInput(INPUT_B)) {
		PlaySoundMem(GetSound(SE_Key_Remove), DX_PLAYTYPE_BACK);
		if (input_i > 0)buf[--input_i] = '_';
	}
}

int Ranking::GetRankingNum(int num) {
	if (g_Ranking[num].no < 6) {
		int no = g_Ranking[num].no;
		return no;
	}
	return 0;
}

char* Ranking::GetRankingName(int num) {
	bool checkflg = false;
	if (0 <= num && num <= 6) {
		for (int i = 0; i < 9; i++) {
			if (isalnum(g_Ranking[num].name[i]) != 0)checkflg = true;
		}
		if (checkflg == true) {
			char* name = g_Ranking[num].name;
			return name;
		}
	}
	return 0;
}

long Ranking::GetRankingScore(int num) {
	long score = 0;;
	if (0 <= num && num <= 6) {
		if ((-99999 >= g_Ranking[num].score && g_Ranking[num].score <= 99999) && (0 <= num && num <= 6))
			score = g_Ranking[num].score;
		return score;
	}
	return 0;
}

int Ranking::ReadRanking(void)
{
	FILE* fp;
#pragma warning(disable:4996)

	//ファイルオープン
	if ((fp = fopen("dat/rankingdata.txt", "r")) == NULL) {
		//エラー処理
		printf("Ranking Data Error\n");
		return -1;
	}

	//ランキングデータ配分列データを読み込む
	for (int i = 0; i < RANKING_DATA; i++) {

		fscanf_s(fp, "%2d %9s %5d\n", &g_Ranking[i].no, &g_Ranking[i].name, 10, &g_Ranking[i].score);
	}

	//ファイルクローズ
	fclose(fp);

	return 0;
}


int  Ranking::SaveRanking(void)
{
	FILE* fp;
#pragma warning(disable:4996)

	// ファイルオープン
	if ((fp = fopen("dat/rankingdata.txt", "w")) == NULL) {
		/* エラー処理 */
		printf("Ranking Data Error\n");
		return -1;
	}

	// ランキングデータ分配列データを書き込む
	for (int i = 0; i < RANKING_DATA; i++) {
		fprintf(fp, "%2d %9s %5d\n", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
	}

	//ファイルクローズ
	fclose(fp);

	return 0;

}


void Ranking::SortRanking(void)
{
	int i, j;
	RankingData work;

	// 選択法ソート
	for (i = 0; i < RANKING_DATA - 1; i++) {
		for (j = i + 1; j < RANKING_DATA; j++) {
			if (g_Ranking[i].score <= g_Ranking[j].score) {
				work = g_Ranking[i];
				g_Ranking[i] = g_Ranking[j];
				g_Ranking[j] = work;
			}
		}
	}

	// 順位付け
	for (i = 0; i < RANKING_DATA; i++) {
		g_Ranking[i].no = 1;
	}
	// 得点が同じ場合は、同じ順位とする
	// 同順位があった場合の次の順位はデータ個数が加算された順位とする
	for (i = 0; i < RANKING_DATA - 1; i++) {
		for (j = i + 1; j < RANKING_DATA; j++) {
			if (g_Ranking[i].score > g_Ranking[j].score) {
				g_Ranking[j].no++;
			}
		}
	}
}