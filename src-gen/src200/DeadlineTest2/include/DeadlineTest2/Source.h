#ifndef _source_H
#define _source_H
#ifndef _SOURCE_H // necessary for arduino-cli, which automatically includes headers that are not used
#ifndef TOP_LEVEL_PREAMBLE_247575268_H
#define TOP_LEVEL_PREAMBLE_247575268_H
/*Correspondence: Range: [(8, 4), (46, 1)) -> Range: [(0, 0), (38, 1)) (verbatim=true; src=/home/yu/lf-test/src200/DeadlineTest2.lf)*/#define DUMMY_LOOP_COUNT 1000
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
