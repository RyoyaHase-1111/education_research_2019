//joycon.c

#pragma comment(lib, "winmm.lib")

/*時間データの取得*/
#include "time.h"

#include<stdio.h>
#include <windows.h> 
#include <mmsystem.h>

/*VSでfopenを使うためのおまじない*/
#pragma warning(disable: 4996)

int joy_g;


void joy_fanc(){

	char filename[50];
    
	FILE *fp;

	double joy_time_s = 203, joy_time_e = 206;
	int joy_active = 0;
	int i = 0;
	int ans[20];

	JOYINFOEX joy;
	joy.dwSize = sizeof JOYINFOEX;
	joy.dwFlags = JOY_RETURNALL;

	sprintf(filename, "%s_ans.csv", timepoints);
	fp = fopen(filename, "a");

	while (1) {

		if (stop) break;

		if (joy_time_s <= get_elapsed(starttime_all)) {
			joy_g = -1;
			joy_active = 1;
			while (1) {
				if (joyGetPosEx(JOYSTICKID1, &joy) == JOYERR_NOERROR) {
					if (joy.dwButtons == 16) {
						joy_g = 2;
						joy.dwButtons = NULL;
					}

					if (joy.dwButtons == 32) {
						joy_g = 1;
						joy.dwButtons = NULL;
					}
				}
				if (joy_time_e <= get_elapsed(starttime_all))break;

			}
		}


		if (joy_active == 1) {

			fprintf(fp, "%d\n", joy_g);

			joy_time_s = joy_time_s + 16;
			joy_time_e = joy_time_e + 16;
			joy_active = 0;

			ans[i] = joy_g;

			printf("%d",i);

			if (i == 19)break;

			i++;
	
		}

		joy_g = 0;


		

	}

	stop = 1;
}