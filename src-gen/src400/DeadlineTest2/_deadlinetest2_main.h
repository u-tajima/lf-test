#ifndef _DEADLINETEST2_MAIN_H
#define _DEADLINETEST2_MAIN_H
#include "include/core/reactor.h"
#include "_source.h"
#include "_destination.h"
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
    struct self_base_t base;
#line 53 "/home/yu/lf-test/src-gen/src400/DeadlineTest2/_deadlinetest2_main.h"
#line 54 "/home/yu/lf-test/src-gen/src400/DeadlineTest2/_deadlinetest2_main.h"
} _deadlinetest2_main_main_self_t;
_deadlinetest2_main_main_self_t* new__deadlinetest2_main();
#endif // _DEADLINETEST2_MAIN_H
