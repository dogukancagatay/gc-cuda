#ifndef TIMING_H
#define TIMING_H
#include <stdio.h>
#include <time.h>

double calc_elapsed_time(struct timespec tsi, struct timespec tsf);

double calc_elapsed_time(struct timespec tsi, struct timespec tsf){
    double elaps_s = difftime(tsf.tv_sec, tsi.tv_sec);
    long elaps_ns = tsf.tv_nsec - tsi.tv_nsec;
    return elaps_s + ((double)elaps_ns) / 1.0e9;
}
#endif
