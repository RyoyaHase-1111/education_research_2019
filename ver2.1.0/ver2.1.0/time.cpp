//time.c
#include <stdio.h> 
#include <time.h>

int stop;

//時刻(グローバル関数)
char timepoints[20];

//開始時間(グローバル関数)
clock_t starttime_all;

//現在の時刻を取得
void get_timepoints(char* dt) {
	//時間データ
	struct tm tm;
	time_t t = time(NULL);
	localtime_s(&tm, &t);

	sprintf_s(dt, 20, "%d%d%d%d%d_%d%d%d%d%d%d",
		tm.tm_year + 1900, (tm.tm_mon + 1) / 10, (tm.tm_mon + 1) % 10, tm.tm_mday / 10, tm.tm_mday % 10,
		tm.tm_hour / 10, tm.tm_hour % 10, tm.tm_min / 10, tm.tm_min % 10, tm.tm_sec / 10, tm.tm_sec % 10);

	starttime_all = clock();
}


//経過時間を取得
double get_elapsed(clock_t starttime) {

	double elapsed;

	clock_t nawtime = clock();

	elapsed = (double)(nawtime - starttime) / CLOCKS_PER_SEC;
	return elapsed;

}

