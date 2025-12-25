#include "include/api/schedule.h"
#include <string.h>
#include "low_level_platform/api/low_level_platform.h"
#include "include/DeadlineTest2/Source.h"
#include "_source.h"
// ***** Start of method declarations.
// ***** End of method declarations.
#include "include/api/reaction_macros.h"
void _sourcereaction_function_0(void* instance_args) {
    _source_self_t* self = (_source_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _source_y_t* y = &self->_lf_y;
    #line 58 "/home/u-tajima/lf-test/src500/DeadlineTest2.lf"
    if (s_log_idx >= MAX_SAMPLES) return;
    
    // 【修正】時間の基準を統一 (get_time_nsを使用)
    long logical_fire_ns = lf_time_logical();
    long reaction_start_ns = get_time_ns();
    
    long scheduler_latency_ns = reaction_start_ns - logical_fire_ns;
    
    // 送信時刻も physical time
    long send_time_ns = reaction_start_ns; 
    
    // ログに保存 (printfはしない)
    s_logs[s_log_idx].scheduler_latency_ns = scheduler_latency_ns;
    s_logs[s_log_idx].send_time_ns = send_time_ns;
    s_logs[s_log_idx].count = self->count;
    s_logs[s_log_idx].print_ts_sec = send_time_ns / 1000000000L;
    s_logs[s_log_idx].print_ts_nsec = send_time_ns % 1000000000L;
    
    // 負荷ループ
    volatile double x_dummy = 0;
    for (int i = 0; i < DUMMY_LOOP_COUNT; i++) {
        x_dummy += i * 0.0001;
    }
    
    // 周期ジッタ計算
    if (self->last_send_time >= 0) {
        long interval_ns = send_time_ns - self->last_send_time;
        s_logs[s_log_idx].interval_ns = interval_ns;
        s_logs[s_log_idx].has_interval = 1;
    } else {
        s_logs[s_log_idx].has_interval = 0;
    }
    
    s_log_idx++;
    self->last_send_time = send_time_ns;
    
    lf_set(y, send_time_ns);
    (self->count)++;
#line 52 "/home/u-tajima/lf-test/src-gen/src500/DeadlineTest2/_source.c"
}
#include "include/api/reaction_macros_undef.h"
_source_self_t* new__source() {
    _source_self_t* self = (_source_self_t*)lf_new_reactor(sizeof(_source_self_t));
    self->_lf__reaction_0.number = 0;
    self->_lf__reaction_0.function = _sourcereaction_function_0;
    self->_lf__reaction_0.self = self;
    self->_lf__reaction_0.deadline_violation_handler = NULL;
    self->_lf__reaction_0.STP_handler = NULL;
    self->_lf__reaction_0.name = "?";
    self->_lf__reaction_0.mode = NULL;
    self->_lf__t.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__t.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__t_reactions[0] = &self->_lf__reaction_0;
    self->_lf__t.reactions = &self->_lf__t_reactions[0];
    self->_lf__t.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__t.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__t.is_timer = true;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__t.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    return self;
}
