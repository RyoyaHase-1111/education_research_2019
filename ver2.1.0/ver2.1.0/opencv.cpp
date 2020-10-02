//opencv.c

/*時間データの取得*/
#include "time.h"

//心拍データ(ECG)
#include "Heartbeat_Elect.h"

//tobiiデータ
#include "tobii.h"

//joycon
#include "joycon.h"

#include <opencv2\opencv.hpp>

#include <windows.h> 

#define ImageWidth 800
#define ImageHeight 400


 //------------描画関数　OpenCVベース（直線）-----------
static void line(cv::Mat & Image, int x1, int y1, int x2, int y2, int R, int G, int B, int width) {
	cv::line(Image, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(B, G, R), width, CV_AA);
}

//------------描画関数　OpenCVベース（円形）-----------
static void circle(cv::Mat & Image, int cx, int cy, int radius, int R, int G, int B, int width) {
	cv::circle(Image, cv::Point(cx, cy), radius, cv::Scalar(B, G, R), width, CV_AA);
}




void ECG200(){

	int i = 0, j = 0;

	cv::Mat ECGWindow200(ImageHeight, ImageWidth, CV_8UC3, cv::Scalar(255, 255, 255));

	cv::Mat clear(ImageHeight, ImageWidth, CV_8UC3, cv::Scalar(255, 255, 255));

	while (1) {

		if (stop) break;

		if (i % 200 == 0) {

			ECGWindow200.clone() = clear;

			ECGWindow200 = cv::Scalar(255, 255, 255);

		}
	
	    line(ECGWindow200, i % 200*4, j / -2 + 400, i % 200*4 + 4, estr_g / -2 + 400, 0, 0, 0, 1);

		j=estr_g;

		i++;

		cv::imshow("心電図(詳細)", ECGWindow200);
		cv::moveWindow("心電図(詳細)", 145, 80);
		//これまで記述した描画イベントを一括処理する（引数は待ち時間）
		char key = cv::waitKey(1);
		if (key == 'q') break; //ｑを押したら止まる
	}
}


void ECG800() {

	int i = 0, j = 0;

	cv::Mat ECGWindow800(ImageHeight, ImageWidth, CV_8UC3, cv::Scalar(255, 255, 255));

	cv::Mat clear(ImageHeight, ImageWidth, CV_8UC3, cv::Scalar(255, 255, 255));

	while (1) {

		if (stop) break;

		if (i % 800 == 0) {

			ECGWindow800.clone() = clear;

			ECGWindow800 = cv::Scalar(255, 255, 255);

		}

		line(ECGWindow800, i % 800, j / -2 + 400, i % 800+1, estr_g / -2 + 400, 0, 0, 0, 1);

		j = estr_g;

		i++;

		cv::imshow("心電図(大略)", ECGWindow800);
		cv::moveWindow("心電図(大略)", 955, 80);
		//これまで記述した描画イベントを一括処理する（引数は待ち時間）
		char key = cv::waitKey(1);
		if (key == 'q') break; //ｑを押したら止まる
	}
}

void tobiiv() {

	//透明度指定用
	double alpha = 0;
	int alphaflap = 0;

	int leftflap = 30;
	int rightflap = 30;

	cv::Mat tobiivv(ImageHeight, ImageWidth * 2, CV_8UC3, cv::Scalar(255, 255, 255));

	cv::Mat out(ImageHeight , ImageWidth*2, CV_8UC3, cv::Scalar(255, 255, 255));

	cv::Mat clear(ImageHeight, ImageWidth*2, CV_8UC3, cv::Scalar(255, 255, 255));

	while (1) {

		if (stop) break;

		out.clone() = clear;

		out = cv::Scalar(255, 255, 255);

			line(tobiivv, ImageWidth, -10, ImageWidth, ImageHeight+10, 0, 0, 0, 1);
			line(out, ImageWidth, -10, ImageWidth, ImageHeight+10, 0, 0, 0, 1);

			line(tobiivv, 1340, -10, 1340, ImageHeight + 10, 0, 0, 0, 1);
			line(out, 1340, -10, 1340, ImageHeight + 10, 0, 0, 0, 1);

			line(tobiivv, 1340, ImageHeight / 3, ImageWidth * 2, ImageHeight/3, 0, 0, 0, 1);
			line(out, 1340, ImageHeight / 3, ImageWidth * 2, ImageHeight /3, 0, 0, 0, 1);

		//目線データ-----------------------------------------------------------------------------------------------
			cv::rectangle(tobiivv, cv::Point(175, 75), cv::Point(625, 325), cv::Scalar(0, 0, 0), 1, CV_AA);
			cv::rectangle(out, cv::Point(175, 75), cv::Point(625, 325), cv::Scalar(0, 0, 0), 1, CV_AA);


		if (eyedata == 1) {
			
			cv::rectangle(tobiivv, cv::Point(176, 76), cv::Point(624, 324), cv::Scalar(255, 255, 255), -1, CV_AA);

			cv::rectangle(out, cv::Point(176, 76), cv::Point(624, 324), cv::Scalar(255, 255, 0), -1, CV_AA);

		}
		else {

			cv::rectangle(tobiivv, cv::Point(0, 0), cv::Point(174, 399), cv::Scalar(255, 255, 255), -1, CV_AA);
			cv::rectangle(tobiivv, cv::Point(626, 0), cv::Point(799, 399), cv::Scalar(255, 255, 255), -1, CV_AA);
			cv::rectangle(tobiivv, cv::Point(174, 0), cv::Point(626, 74), cv::Scalar(255, 255, 255), -1, CV_AA);
			cv::rectangle(tobiivv, cv::Point(174, 326), cv::Point(626, 400), cv::Scalar(255, 255, 255), -1, CV_AA);

			cv::rectangle(out, cv::Point(0, 0), cv::Point(174, 399), cv::Scalar(51, 51,255), -1, CV_AA);
			cv::rectangle(out, cv::Point(626, 0), cv::Point(799, 399), cv::Scalar(51, 51, 255), -1, CV_AA);
			cv::rectangle(out, cv::Point(174, 0), cv::Point(626, 74), cv::Scalar(51, 51, 255), -1, CV_AA);
			cv::rectangle(out, cv::Point(174, 326), cv::Point(626, 400), cv::Scalar(51, 51, 255), -1, CV_AA);

		}
		//----------------------------------------------------------------------------------------------------------

		//tobiiの識別-----------------------------------------------------------------------------------------------
		cv::putText(tobiivv, "Tobii", cv::Point(1350, 30), CV_FONT_HERSHEY_TRIPLEX, 1,cv::Scalar(0, 0, 0));
		cv::putText(out, "Tobii", cv::Point(1350, 30), CV_FONT_HERSHEY_TRIPLEX, 1, cv::Scalar(0, 0, 0));

		if (headpositiondata == 1) {
			cv::circle(tobiivv, cv::Point(1465, 80), 20, cv::Scalar(255, 255, 0), -1, CV_AA);
			cv::circle(out, cv::Point(1465, 80), 20, cv::Scalar(255, 255, 0), -1, CV_AA);
		}
		else {
			cv::circle(tobiivv, cv::Point(1465, 80), 20, cv::Scalar(51, 51, 255), -1, CV_AA);
			cv::circle(out, cv::Point(1465, 80), 20, cv::Scalar(51, 51, 255), -1, CV_AA);
		}
		//-----------------------------------------------------------------------------------------------------------

		//瞬き-------------------------------------------------------------------------------------------------------

		if (eyepositiondata_left == 1) {
			if (leftflap < 30)leftflap= leftflap+30;
			cv::ellipse(tobiivv, cv::Point2f(950, 180), cv::Size(80, leftflap), 5,0,360, cv::Scalar(0, 0, 0), 1, CV_AA);
			cv::ellipse(out, cv::Point2f(950, 180), cv::Size(80, leftflap), 5, 0, 360, cv::Scalar(0, 0, 0), 1, CV_AA);
		}
		else {
			if (leftflap > 1)leftflap=leftflap-30;
			cv::ellipse(tobiivv, cv::Point2f(950, 180), cv::Size(80, leftflap), 5, 0, 360, cv::Scalar(0, 0, 0), 1, CV_AA);
			cv::ellipse(out, cv::Point2f(950, 180), cv::Size(80, leftflap), 5, 0, 360, cv::Scalar(0, 0, 0), 1, CV_AA);
		}

		if (eyepositiondata_right == 1) {
			if (rightflap < 30)rightflap = rightflap + 30;
			cv::ellipse(tobiivv, cv::Point2f(1200, 180), cv::Size(80, rightflap), -5, 0, 360, cv::Scalar(0, 0, 0), 1, CV_AA);
			cv::ellipse(out, cv::Point2f(1200, 180), cv::Size(80, rightflap), -5, 0, 360, cv::Scalar(0, 0, 0), 1, CV_AA);
		}
		else {
			if (rightflap > 0)rightflap = rightflap - 30;
			cv::ellipse(tobiivv, cv::Point2f(1200, 180), cv::Size(80, rightflap), -5, 0, 360, cv::Scalar(0, 0, 0), 1, CV_AA);
			cv::ellipse(out, cv::Point2f(1200, 180), cv::Size(80, rightflap), -5, 0, 360, cv::Scalar(0, 0, 0), 1, CV_AA);
		}


		//-----------------------------------------------------------------------------------------------------------

		//tobiiの識別-----------------------------------------------------------------------------------------------
		std::string ALLtime = std::to_string(int(get_elapsed(starttime_all)));
		cv::putText(tobiivv, ALLtime, cv::Point(1400, 280), CV_FONT_HERSHEY_TRIPLEX, 1, cv::Scalar(0, 0, 0));
		cv::putText(out, ALLtime, cv::Point(1400, 280), CV_FONT_HERSHEY_TRIPLEX, 1, cv::Scalar(0, 0, 0));
		//-----------------------------------------------------------------------------------------------------------


		if (stop) break;

		if (alphaflap == 0) {
			alpha = alpha + 0.005;
			if (alpha >= 0.5)alphaflap = 1;
		}
		else {
			alpha = alpha - 0.005;
			if (alpha <= 0.1)alphaflap = 0;
		}

		cv::addWeighted(out, alpha, tobiivv, 1 - alpha, 0, tobiivv);

		
		cv::imshow("Tobii eye tracker 4c", tobiivv);
		cv::moveWindow("Tobii eye tracker 4c", 150, 580);
		//これまで記述した描画イベントを一括処理する（引数は待ち時間）
		char key = cv::waitKey(1);
		if (key == 'q') break; //ｑを押したら止まる
	}

}

void movie() {

	// ディスプレイサイズを取得
	int DisplayWidth = GetSystemMetrics(SM_CXSCREEN);
	int DisplayHeight = GetSystemMetrics(SM_CYSCREEN);

	//動画の名前
	cv::VideoCapture cap("B3s_470s.mov");


	int max_frame = 30 * 580;
	double fps = cap.get(CV_CAP_PROP_FPS);

	int freme = 0;
	double freme_time = 10;

	cv::Mat MovieWindow(DisplayHeight, DisplayWidth, CV_8UC3, cv::Scalar(255, 255, 255));

	while (1) {

		if (get_elapsed(starttime_all) >= 10) break;
	}

	while (1) {

		if (stop) break;

		if (freme_time <= get_elapsed(starttime_all)) {

			if (freme < max_frame) {

				cap >> MovieWindow; //1フレーム分取り出してimgに保持させる

				if (freme % 2 == 0) {

					freme_time = freme_time + 1.0 / 15;

					//joyconのデータを書き込み
					if (joy_g == 2) {

						circle(MovieWindow, 250, 240, 50, 255, 255, 255, -1);

					}
					else if (joy_g == 1) {

						circle(MovieWindow, 1670, 240, 50, 255, 255, 255, -1);

					}

					cv::imshow("課題活動", MovieWindow);
					char key = cv::waitKey(1);
					if (key == 'q') break;
				}

				freme++;

			}
			else {
				//freme < max_frameでbreak
				stop = 1;
				break;
			}
		}
		if (stop) break;
	}
}
