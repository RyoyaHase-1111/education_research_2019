//time.h
#pragma once

#include <time.h>

extern int stop;

////現在の時刻を取得
void get_timepoints(char* timepoints);
extern char timepoints[20];

//経過時間を取得
double get_elapsed(clock_t starttime);
extern clock_t starttime_all;
