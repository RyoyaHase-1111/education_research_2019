//tobii.h

#include "time.h"

#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include <tobii.h>
#include <tobii_streams.h>


/*VSでfopenを使うためのおまじない*/
#pragma warning(disable: 4996)

static FILE *fp_tobii_eye, *fp_tobii_eyep,*fp_tobii_headp;
static clock_t starttime_e, starttime_h, left_closetime, right_closetime;

int eyedata;
int eyepositiondata_left, eyepositiondata_right;
double wink_left, wink_right;
int headpositiondata;


//頭のポジション
void head_pose_callback(tobii_head_pose_t const* head_pose, void* user_data)
{
	if (head_pose->position_validity == TOBII_VALIDITY_VALID) {
		fprintf(fp_tobii_headp,"%lf,Position:,%f,%f,%f,",
			get_elapsed(starttime_e),
			head_pose->position_xyz[0],
			head_pose->position_xyz[1],
			head_pose->position_xyz[2]);

		if (headpositiondata == 0) {
			headpositiondata = 1;
		}
	}
	else {
		fprintf(fp_tobii_headp, "%lf,Position:,-1,-1,-1,",
			get_elapsed(starttime_e));

		if (headpositiondata == 1) {
			headpositiondata = 0;
		}
	}

	if (head_pose->position_validity == TOBII_VALIDITY_VALID) {
		fprintf(fp_tobii_headp, "Rotation:,%f,%f,%f\n",
			head_pose->rotation_xyz[0],
			head_pose->rotation_xyz[1],
			head_pose->rotation_xyz[2]);
	}
	else {
		fprintf(fp_tobii_headp, "Rotation:,-1,-1,-1\n");
	}

}


//目の位置
void eye_position_callback(tobii_eye_position_normalized_t const* eye_pos, void* user_data)
{
	if (eye_pos->left_validity == TOBII_VALIDITY_VALID) {
		fprintf(fp_tobii_eyep, "%lf,Left:,%f,%f,%f,",
			get_elapsed(starttime_e),
			eye_pos->left_xyz[0],
			eye_pos->left_xyz[1],
			eye_pos->left_xyz[2]);

		if (eyepositiondata_left == 0) {
			eyepositiondata_left = 1;
			wink_left = get_elapsed(left_closetime);
		}
	}
	else {
		fprintf(fp_tobii_eyep, "%lf,Left:,-1,-1,-1,", get_elapsed(starttime_e));
		if (eyepositiondata_left == 1) {
			eyepositiondata_left = 0;
			left_closetime = clock();
		}
	}

	if (eye_pos->right_validity == TOBII_VALIDITY_VALID) {

		fprintf(fp_tobii_eyep, "Right:,%f,%f,%f\n",
			eye_pos->right_xyz[0],
			eye_pos->right_xyz[1],
			eye_pos->right_xyz[2]);

		if (eyepositiondata_right == 0) {
			eyepositiondata_right = 1;
			wink_right = get_elapsed(right_closetime);
		}
	}
	else {
		fprintf(fp_tobii_eyep, "Right:,-1,-1,-1\n");
		if (eyepositiondata_right == 1) {
			eyepositiondata_right = 0;
			right_closetime = clock();
		}
	}


}


//視線計測
static void gaze_point_callback(tobii_gaze_point_t const* gaze_point, void* user_data)
{
	double x, y;
	if (gaze_point->validity == TOBII_VALIDITY_VALID) {
		fprintf(fp_tobii_eye, "%f,%f,%f\n",
			get_elapsed(starttime_e),
			gaze_point->position_xy[0],
			gaze_point->position_xy[1]);

		x = double(gaze_point->position_xy[0]);
		y = double(gaze_point->position_xy[1]);




		if (0 <= x && x <= 1 && 0 <= y && y <= 1) {
			if (eyedata == 0) {
				eyedata = 1;
			}
		}
		else {
			if (eyedata == 1) {
				eyedata = 0;
			}
		}
	}
	else {
		fprintf(fp_tobii_eye, "%f,-1,-1\n",
			get_elapsed(starttime_e));

		if (eyedata == 1) {
			eyedata = 0;
		}
	}

}


static void url_receiver(char const* url, void* user_data)
{
	char* buffer = (char*)user_data;
	if (*buffer != '\0') return; // only keep first value

	if (strlen(url) < 256)
		strcpy(buffer, url);
}


int tobii()
{
	/*ファイル用*/
	char filename[3][50];

	//ファイルを作成---------------------------------------------------
	sprintf(filename[0], "%s_eye.csv", timepoints);
	sprintf(filename[1], "%s_eyep.csv", timepoints);
	sprintf(filename[2], "%s_headp.csv", timepoints);
	fp_tobii_eye = fopen(filename[0], "a");
	fp_tobii_eyep = fopen(filename[1], "a");
	fp_tobii_headp = fopen(filename[2], "a");
	//-----------------------------------------------------------------

	eyedata = 0;
	eyepositiondata_left = 0;
	eyepositiondata_right = 0;
	wink_left = 0;
	wink_right = 0;
	headpositiondata = 0;

	tobii_api_t* api;
	tobii_error_t error = tobii_api_create(&api, NULL, NULL);
	assert(error == TOBII_ERROR_NO_ERROR);

	char url[256] = { 0 };
	error = tobii_enumerate_local_device_urls(api, url_receiver, url);
	assert(error == TOBII_ERROR_NO_ERROR && *url != '\0');

	tobii_device_t* device;
	error = tobii_device_create(api, url, TOBII_FIELD_OF_USE_INTERACTIVE, &device);
	assert(error == TOBII_ERROR_NO_ERROR);

	//頭の位置計測を有効化
	error = tobii_head_pose_subscribe(device, head_pose_callback, 0);
	assert(error == TOBII_ERROR_NO_ERROR);

	//目の位置計測を有効化
	error = tobii_eye_position_normalized_subscribe(device, eye_position_callback, 0);
	assert(error == TOBII_ERROR_NO_ERROR);

	//目線の計測を有効化
	error = tobii_gaze_point_subscribe(device, gaze_point_callback, 0);
	assert(error == TOBII_ERROR_NO_ERROR);

	while (1) {
		if (get_elapsed(starttime_all) >= 10) break;
	}

	starttime_e = clock();

	while (1)
	{
		error = tobii_wait_for_callbacks(1, &device);
		assert(error == TOBII_ERROR_NO_ERROR || error == TOBII_ERROR_TIMED_OUT);

		error = tobii_device_process_callbacks(device);
		assert(error == TOBII_ERROR_NO_ERROR);

		if (stop) break;
	}

	error = tobii_gaze_point_unsubscribe(device);
	assert(error == TOBII_ERROR_NO_ERROR);

	error = tobii_device_destroy(device);
	assert(error == TOBII_ERROR_NO_ERROR);

	error = tobii_api_destroy(api);
	assert(error == TOBII_ERROR_NO_ERROR);

	fclose(fp_tobii_eye);
	fclose(fp_tobii_eyep);
	fclose(fp_tobii_headp);

	return 1;

}

