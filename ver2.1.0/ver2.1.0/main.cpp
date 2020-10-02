//���ԃf�[�^�̎擾
#include "time.h"

//�S���f�[�^(ECG)
#include "Heartbeat_Elect.h"

//tobii
#include "tobii.h"

//opencv
#include "opencv.h"

////joy�R��
#include "joycon.h"

//�����ݒ�
#include <stdio.h>
#include <windows.h> 

//�X���b�h
#include <thread>
#include <vector>

//��~�p
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
	//ECG�N��----------------------------------------------------------
	wchar_t *ECG_port = L"COM3";
	Heartbeat_Elect(ECG_port);
	//-----------------------------------------------------------------
}

void ECGv1() {
	//gaze_point�N��----------------------------------------------------------
	ECG200();
	//-----------------------------------------------------------------
}

void ECGv2() {
	//gaze_point�N��----------------------------------------------------------
	ECG800();
	//-----------------------------------------------------------------
}

void TobiiFunc() {
	//gaze_point�N��----------------------------------------------------------
	tobii();
	//-----------------------------------------------------------------
}

void Tobiiv() {
	//gaze_point�N��----------------------------------------------------------
	tobiiv();
	//-----------------------------------------------------------------
}

void movieFunc() {
	//gaze_point�N��----------------------------------------------------------
	movie();
	//-----------------------------------------------------------------
}

void joyf() {
	//gaze_point�N��----------------------------------------------------------
	joy_fanc();
	//-----------------------------------------------------------------
}


int main() { 

	stop = 0;

	std::thread Stop(StopFunc);

	//�X�^�[�g--------------------------------------------------------
	printf("stert y/n \n");
	while (1) {
		if (GetAsyncKeyState(89) || GetAsyncKeyState(13)) {
			printf("stert naw\n\n");
			break;
		}
	}
	//-----------------------------------------------------------------

	//���݂̎������擾-------------------------------------------------
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