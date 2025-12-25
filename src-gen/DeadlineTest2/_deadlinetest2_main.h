#ifndef _DEADLINETEST2_MAIN_H
#define _DEADLINETEST2_MAIN_H
#include "include/core/reactor.h"
#include "_source.h"
#include "_destination.h"
#ifndef TOP_LEVEL_PREAMBLE_2066945445_H
#define TOP_LEVEL_PREAMBLE_2066945445_H
#define DUMMY_LOOP_COUNT 0
#include <time.h>
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
static inline long get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000000000L + ts.tv_nsec;
}
#endif // TOP_LEVEL_PREAMBLE_2066945445_H


typedef struct {
    struct self_base_t base;
#line 24 "/home/u-tajima/lf-test/src-gen/DeadlineTest2/_deadlinetest2_main.h"
#line 25 "/home/u-tajima/lf-test/src-gen/DeadlineTest2/_deadlinetest2_main.h"
} _deadlinetest2_main_main_self_t;
_deadlinetest2_main_main_self_t* new__deadlinetest2_main();
#endif // _DEADLINETEST2_MAIN_H
