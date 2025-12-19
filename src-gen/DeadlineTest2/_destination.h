#ifndef _DESTINATION_H
#define _DESTINATION_H
#include "include/core/reactor.h"
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
} _destination_x_t;
typedef struct {
    struct self_base_t base;
    #line 59 "/home/yu/lf-test/src/DeadlineTest2.lf"
    interval_t timeout;
#line 36 "/home/yu/lf-test/src-gen/DeadlineTest2/_destination.h"
    #line 61 "/home/yu/lf-test/src/DeadlineTest2.lf"
    int count;
    #line 62 "/home/yu/lf-test/src/DeadlineTest2.lf"
    int deadline_miss_count;
    #line 63 "/home/yu/lf-test/src/DeadlineTest2.lf"
    int total_received;
    #line 64 "/home/yu/lf-test/src/DeadlineTest2.lf"
    int max_count;
#line 45 "/home/yu/lf-test/src-gen/DeadlineTest2/_destination.h"
    _destination_x_t* _lf_x;
    // width of -2 indicates that it is not a multiport.
    int _lf_x_width;
    // Default input (in case it does not get connected)
    _destination_x_t _lf_default__x;
    reaction_t _lf__reaction_0;
    trigger_t _lf__x;
    reaction_t* _lf__x_reactions[1];
    #ifdef FEDERATED
    
    #endif // FEDERATED
} _destination_self_t;
_destination_self_t* new__destination();
#endif // _DESTINATION_H
