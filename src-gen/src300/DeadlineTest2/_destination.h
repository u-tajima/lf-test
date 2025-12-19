#ifndef _DESTINATION_H
#define _DESTINATION_H
#include "include/core/reactor.h"
#ifndef TOP_LEVEL_PREAMBLE_247575268_H
#define TOP_LEVEL_PREAMBLE_247575268_H
#define DUMMY_LOOP_COUNT 1000
#include <time.h>
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_SAMPLES 305

// ログ用構造体
typedef struct {
    long scheduler_latency_ns;
    long print_ts_sec;
    long print_ts_nsec;
    int count;
    long send_time_ns;
    long interval_ns;
    int has_interval;
} SourceLog;

typedef struct {
    long print_ts_sec;
    long print_ts_nsec;
    long send_time_ns;
    long recv_time_ns;
    long error_ns;
    int is_miss;
} DestLog;

// 変数は extern で宣言だけする (リンクエラー防止)
extern SourceLog s_logs[MAX_SAMPLES];
extern DestLog d_logs[MAX_SAMPLES];
extern int s_log_idx;
extern int d_log_idx;

static inline long get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000000000L + ts.tv_nsec;
}
#endif // TOP_LEVEL_PREAMBLE_247575268_H
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
    #line 99 "/home/yu/lf-test/src300/DeadlineTest2.lf"
    interval_t timeout;
#line 65 "/home/yu/lf-test/src-gen/src300/DeadlineTest2/_destination.h"
    #line 101 "/home/yu/lf-test/src300/DeadlineTest2.lf"
    int count;
    #line 102 "/home/yu/lf-test/src300/DeadlineTest2.lf"
    int deadline_miss_count;
    #line 103 "/home/yu/lf-test/src300/DeadlineTest2.lf"
    int total_received;
    #line 104 "/home/yu/lf-test/src300/DeadlineTest2.lf"
    int max_count;
#line 74 "/home/yu/lf-test/src-gen/src300/DeadlineTest2/_destination.h"
    _destination_x_t* _lf_x;
    // width of -2 indicates that it is not a multiport.
    int _lf_x_width;
    // Default input (in case it does not get connected)
    _destination_x_t _lf_default__x;
    reaction_t _lf__reaction_0;
    reaction_t _lf__reaction_1;
    trigger_t _lf__shutdown;
    reaction_t* _lf__shutdown_reactions[1];
    trigger_t _lf__x;
    reaction_t* _lf__x_reactions[1];
    #ifdef FEDERATED
    
    #endif // FEDERATED
} _destination_self_t;
_destination_self_t* new__destination();
#endif // _DESTINATION_H
