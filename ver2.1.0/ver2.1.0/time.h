//time.h
#pragma once

#include <time.h>

extern int stop;

////���݂̎������擾
void get_timepoints(char* timepoints);
extern char timepoints[20];

//�o�ߎ��Ԃ��擾
double get_elapsed(clock_t starttime);
extern clock_t starttime_all;
