#ifndef _SOURCE_H
#define _SOURCE_H
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
} _source_y_t;
typedef struct {
    struct self_base_t base;
    #line 50 "/home/yu/lf-test/src300/DeadlineTest2.lf"
    interval_t period;
#line 65 "/home/yu/lf-test/src-gen/src300/DeadlineTest2/_source.h"
    #line 54 "/home/yu/lf-test/src300/DeadlineTest2.lf"
    int count;
    #line 55 "/home/yu/lf-test/src300/DeadlineTest2.lf"
    int64_t last_send_time;
#line 70 "/home/yu/lf-test/src-gen/src300/DeadlineTest2/_source.h"
    _source_y_t _lf_y;
    int _lf_y_width;
    reaction_t _lf__reaction_0;
    trigger_t _lf__t;
    reaction_t* _lf__t_reactions[1];
} _source_self_t;
_source_self_t* new__source();
#endif // _SOURCE_H
