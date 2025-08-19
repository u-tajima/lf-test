#ifndef _source_H
#define _source_H
#ifndef _SOURCE_H // necessary for arduino-cli, which automatically includes headers that are not used
#ifndef TOP_LEVEL_PREAMBLE_16196099_H
#define TOP_LEVEL_PREAMBLE_16196099_H
/*Correspondence: Range: [(6, 4), (15, 1)) -> Range: [(0, 0), (9, 1)) (verbatim=true; src=/home/tajima/lf-test/src/DeadlineTest2.lf)*/#define DUMMY_LOOP_COUNT 100000
#include <time.h>
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
static inline long get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000000000L + ts.tv_nsec;
}
#endif // TOP_LEVEL_PREAMBLE_16196099_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../include/api/schedule.h"
#include "../include/core/reactor.h"
#ifdef __cplusplus
}
#endif
typedef struct source_self_t{
    self_base_t base; // This field is only to be used by the runtime, not the user.
    interval_t period;
    int count;
    int64_t last_send_time;
    int end[0]; // placeholder; MSVC does not compile empty structs
} source_self_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int64_t value;
    #ifdef FEDERATED
    #ifdef FEDERATED_DECENTRALIZED
    tag_t intended_tag;
    #endif
    interval_t physical_time_of_arrival;
    #endif
} source_y_t;
#endif
#endif
