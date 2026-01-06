#include "../includes/sleep.h"

void UsSleep(long microseconds) {
    struct timespec ts;
    ts.tv_sec = microseconds / 1000000;
    ts.tv_nsec = (microseconds % 1000000) * 1000;
    
    nanosleep(&ts, NULL);
}