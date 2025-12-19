#ifndef _deadlinetest2_main_H
#define _deadlinetest2_main_H
#ifndef _DEADLINETEST2_MAIN_H // necessary for arduino-cli, which automatically includes headers that are not used
#ifndef TOP_LEVEL_PREAMBLE_2066945445_H
#define TOP_LEVEL_PREAMBLE_2066945445_H
/*Correspondence: Range: [(6, 4), (15, 1)) -> Range: [(0, 0), (9, 1)) (verbatim=true; src=/home/yu/lf-test/src/DeadlineTest2.lf)*/#define DUMMY_LOOP_COUNT 0
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


#ifdef __cplusplus
extern "C" {
#endif
#include "../include/api/schedule.h"
#include "../include/core/reactor.h"
#ifdef __cplusplus
}
#endif
typedef struct deadlinetest2_self_t{
    self_base_t base; // This field is only to be used by the runtime, not the user.
    int end[0]; // placeholder; MSVC does not compile empty structs
} deadlinetest2_self_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int64_t value;

} source_y_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int64_t value;

} destination_x_t;
#endif
#endif
