#include "include/api/schedule.h"
#ifdef FEDERATED
#include <string.h>
#endif // FEDERATED
#include "low_level_platform/api/low_level_platform.h"
#include "include/DeadlineTest2/Destination.h"
#include "_destination.h"
// ***** Start of method declarations.
void _destination_method_record_log(void* instance_args, int64_t val, int is_miss);
#define record_log(...) _destination_method_record_log(self, ##__VA_ARGS__)
#line 107 "/home/yu/lf-test/src100/DeadlineTest2.lf"
#include "include/api/reaction_macros.h"
// Implementation of method record_log()
void _destination_method_record_log(void* instance_args, int64_t val, int is_miss) {
    _destination_self_t* self = (_destination_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    #line 108 "/home/yu/lf-test/src100/DeadlineTest2.lf"
    if (d_log_idx >= MAX_SAMPLES) return;
    
    long recv_time_ns = get_time_ns(); // 【修正】physical time
    long send_time_ns = val;
    long error_ns = recv_time_ns - send_time_ns;
    
    d_logs[d_log_idx].print_ts_sec = recv_time_ns / 1000000000L;
    d_logs[d_log_idx].print_ts_nsec = recv_time_ns % 1000000000L;
    d_logs[d_log_idx].send_time_ns = send_time_ns;
    d_logs[d_log_idx].recv_time_ns = recv_time_ns;
    d_logs[d_log_idx].error_ns = error_ns;
    d_logs[d_log_idx].is_miss = is_miss;
    
    d_log_idx++;
}
#include "include/api/reaction_macros_undef.h"
#line 34 "/home/yu/lf-test/src-gen/src100/DeadlineTest2/_destination.c"
#undef record_log
// ***** End of method declarations.
#include "include/api/reaction_macros.h"
#define record_log(...) _destination_method_record_log(self, ##__VA_ARGS__)
void _destinationreaction_function_0(void* instance_args) {
    _destination_self_t* self = (_destination_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _destination_x_t* x = self->_lf_x;
    int x_width = self->_lf_x_width; SUPPRESS_UNUSED_WARNING(x_width);
    #line 125 "/home/yu/lf-test/src100/DeadlineTest2.lf"
    (self->total_received)++;
    
    (self->count)++;
    
    record_log(x->value, 0);
    
    if (self->count >= self->max_count) {
        lf_request_stop();
    }
#line 53 "/home/yu/lf-test/src-gen/src100/DeadlineTest2/_destination.c"
}
void _destination_deadline_function0(void* instance_args) {
    _destination_self_t* self = (_destination_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _destination_x_t* x = self->_lf_x;
    int x_width = self->_lf_x_width; SUPPRESS_UNUSED_WARNING(x_width);
    #line 137 "/home/yu/lf-test/src100/DeadlineTest2.lf"
    (self->total_received)++;
    
    (self->deadline_miss_count)++;
    (self->count)++;
    
    record_log(x->value, 1);
    
    if (self->count >= self->max_count) {
        lf_request_stop();
    }
#line 70 "/home/yu/lf-test/src-gen/src100/DeadlineTest2/_destination.c"
}
#undef record_log
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
#define record_log(...) _destination_method_record_log(self, ##__VA_ARGS__)
void _destinationreaction_function_1(void* instance_args) {
    _destination_self_t* self = (_destination_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    
    #line 150 "/home/yu/lf-test/src100/DeadlineTest2.lf"
    int loop_max = (s_log_idx > d_log_idx) ? s_log_idx : d_log_idx;
    
    for (int i = 0; i < loop_max; i++) {
        // Source側のログ出力
        if (i < s_log_idx) {
            printf("Scheduler latency: %ld ns\n", s_logs[i].scheduler_latency_ns);
            // 必要ならここもコメントアウトを外して詳細出力
            // if (s_logs[i].has_interval) { ... }
        }
    
        // Destination側のログ出力
        if (i < d_log_idx) {
            if (d_logs[i].is_miss) {
                printf("[%ld.%09ld] Destination receives msg, send_time: %ld ns, recv_time: %ld ns, error: %ld ns --Deadline misses--\n", 
                    d_logs[i].print_ts_sec, d_logs[i].print_ts_nsec,
                    d_logs[i].send_time_ns, d_logs[i].recv_time_ns, d_logs[i].error_ns);
            } else {
                printf("[%ld.%09ld] Destination receives msg, send_time: %ld ns, recv_time: %ld ns, error: %ld ns\n", 
                    d_logs[i].print_ts_sec, d_logs[i].print_ts_nsec,
                    d_logs[i].send_time_ns, d_logs[i].recv_time_ns, d_logs[i].error_ns);
            }
        }
    }
    printf("=== Summary ===\n");
    printf("Total received: %d\n", self->total_received);
    printf("Deadline misses: %d\n", self->deadline_miss_count);
    
    // 【重要】出力を確実にするためにフラッシュ
    fflush(stdout);
#line 109 "/home/yu/lf-test/src-gen/src100/DeadlineTest2/_destination.c"
}
#undef record_log
#include "include/api/reaction_macros_undef.h"
_destination_self_t* new__destination() {
    _destination_self_t* self = (_destination_self_t*)lf_new_reactor(sizeof(_destination_self_t));
    // Set input by default to an always absent default input.
    self->_lf_x = &self->_lf_default__x;
    // Set the default source reactor pointer
    self->_lf_default__x._base.source_reactor = (self_base_t*)self;
    self->_lf__reaction_0.number = 0;
    self->_lf__reaction_0.function = _destinationreaction_function_0;
    self->_lf__reaction_0.self = self;
    self->_lf__reaction_0.deadline_violation_handler = &_destination_deadline_function0;
    self->_lf__reaction_0.STP_handler = NULL;
    self->_lf__reaction_0.name = "?";
    self->_lf__reaction_0.mode = NULL;
    self->_lf__reaction_1.number = 1;
    self->_lf__reaction_1.function = _destinationreaction_function_1;
    self->_lf__reaction_1.self = self;
    self->_lf__reaction_1.deadline_violation_handler = NULL;
    self->_lf__reaction_1.STP_handler = NULL;
    self->_lf__reaction_1.name = "?";
    self->_lf__reaction_1.mode = NULL;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__shutdown.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__shutdown_reactions[0] = &self->_lf__reaction_1;
    self->_lf__shutdown.last_tag = NEVER_TAG;
    self->_lf__shutdown.reactions = &self->_lf__shutdown_reactions[0];
    self->_lf__shutdown.number_of_reactions = 1;
    self->_lf__shutdown.is_timer = false;
    self->_lf__x.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__x.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__x_reactions[0] = &self->_lf__reaction_0;
    self->_lf__x.reactions = &self->_lf__x_reactions[0];
    self->_lf__x.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__x.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__x.tmplt.type.element_size = sizeof(int64_t);
    return self;
}
