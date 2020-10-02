//時間データの取得
#include "time.h"

//心拍データ(ECG)
#include "Heartbeat_Elect.h"

//tobii
#include "tobii.h"

//opencv
#include "opencv.h"

////joyコン
#include "joycon.h"

//初期設定
#include <stdio.h>
#include <windows.h> 

//スレッド
#include <thread>
#include <vector>

//停止用
void StopFunc(){
	

	while (1) {
		if (GetAsyncKeyState('Q') ) {
			break;
		}
		if (stop) break;
	}
	stop = 1;
}

void ECGFunc(){
	//ECG起動----------------------------------------------------------
	wchar_t *ECG_port = L"COM3";
	Heartbeat_Elect(ECG_port);
	//-----------------------------------------------------------------
}

void ECGv1() {
	//gaze_point起動----------------------------------------------------------
	ECG200();
	//-----------------------------------------------------------------
}

void ECGv2() {
	//gaze_point起動----------------------------------------------------------
	ECG800();
	//-----------------------------------------------------------------
}

void TobiiFunc() {
	//gaze_point起動----------------------------------------------------------
	tobii();
	//-----------------------------------------------------------------
}

void Tobiiv() {
	//gaze_point起動----------------------------------------------------------
	tobiiv();
	//-----------------------------------------------------------------
}

void movieFunc() {
	//gaze_point起動----------------------------------------------------------
	movie();
	//-----------------------------------------------------------------
}

void joyf() {
	//gaze_point起動----------------------------------------------------------
	joy_fanc();
	//-----------------------------------------------------------------
}


int main() { 

	stop = 0;

	std::thread Stop(StopFunc);

	//スタート--------------------------------------------------------
	printf("stert y/n \n");
	while (1) {
		if (GetAsyncKeyState(89) || GetAsyncKeyState(13)) {
			printf("stert naw\n\n");
			break;
		}
	}
	//-----------------------------------------------------------------

	//現在の時刻を取得-------------------------------------------------
	get_timepoints(timepoints);
	//-----------------------------------------------------------------

	

	
	std::thread ECG(ECGFunc);
	std::thread ECG_v1(ECG200);
	std::thread ECG_v2(ECG800);
	std::thread Tobii(TobiiFunc);
	std::thread Tobiiview(Tobiiv);
	std::thread MOVIE(movieFunc);
	std::thread JOY(joyf);

	Stop.join();
	ECG.join();
	ECG_v1.join();
	ECG_v2.join();
	Tobii.join();
	Tobiiview.join();
	MOVIE.join();
	JOY.join();


}