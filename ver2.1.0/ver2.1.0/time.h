//time.h
#pragma once

#include <time.h>

extern int stop;

////Œ»İ‚Ì‚ğæ“¾
void get_timepoints(char* timepoints);
extern char timepoints[20];

//Œo‰ßŠÔ‚ğæ“¾
double get_elapsed(clock_t starttime);
extern clock_t starttime_all;
