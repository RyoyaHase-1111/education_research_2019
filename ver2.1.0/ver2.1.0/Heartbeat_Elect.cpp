//Heartbeat_Elect.c

/*時間データの取得*/
#include "time.h"

/*初期設定*/
#include<stdio.h>
#include <windows.h> 

/*VSでfopenを使うためのおまじない*/
#pragma warning(disable: 4996)

int estr_g;

void Heartbeat_Elect(wchar_t *port) {

	//関数------------------------------------------------------------
	/*ファイル用*/
	char filename[50];
	clock_t starttime;
	FILE *fp_ECG;
	double interval = 0;

	/*シリアル通信用*/
	HANDLE h;
	int i = 0;
	char sBuf[1];
	char str[100];
	int baudRate = 500000;
	unsigned long nn;
	DCB dcb;


	//-----------------------------------------------------------------

	//ファイルを作成---------------------------------------------------
	sprintf(filename, "%s_ECG.csv", timepoints);
	fp_ECG = fopen(filename, "a");
	//-----------------------------------------------------------------

	//シリアル通信------------------------------------------------------
	h = CreateFile(port,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (h == INVALID_HANDLE_VALUE) {
		printf("ECG Open Error\n");
	}

	GetCommState(h, &dcb); // シリアルポートの状態を取得
	dcb.BaudRate = baudRate;
	dcb.Parity = NOPARITY;
	dcb.fParity = FALSE;
	dcb.StopBits = ONESTOPBIT;
	SetCommState(h, &dcb); // シリアルポートの状態を設定
	//-----------------------------------------------------------------






		//シリアルポートに対して読み込み-----------------------------------
	while (1) {

		if (get_elapsed(starttime_all) >= 10) {
			break;
		}

		ReadFile(h, sBuf, 1, &nn, 0); // シリアルポートに対する読み込み
		if (nn == 1) {
			if (sBuf[0] == 10 || sBuf[0] == 13) { // '\r'や'\n'を受信すると文字列を閉じる
				if (i != 0) {
					/*描画処理(main部分)*/
					str[i] = '\0';
					i = 0;
				}
			}
			else {
				str[i] = sBuf[0];
				i++;
			}
		}
	}

	starttime = clock();

	//シリアルポートに対して読み込み-----------------------------------
	while (1) {


			ReadFile(h, sBuf, 1, &nn, 0); // シリアルポートに対する読み込み

			if (nn == 1) {
				if (sBuf[0] == 10 || sBuf[0] == 13) { // '\r'や'\n'を受信すると文字列を閉じる
					if (i != 0) {
						/*描画処理(main部分)*/
						str[i] = '\0';
						i = 0;

						estr_g = atoi(str);

						fprintf(fp_ECG, "%lf,%s\n",interval, str);
						interval = interval + 0.01;
					}
				}
				else {
					str[i] = sBuf[0];
					i++;
				}
			}

		if (stop) break;
	}
	fclose(fp_ECG);
}
