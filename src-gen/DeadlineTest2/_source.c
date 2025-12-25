#include "include/api/schedule.h"
#ifdef FEDERATED
#include <string.h>
#endif // FEDERATED
#include "low_level_platform/api/low_level_platform.h"
#include "include/DeadlineTest2/Source.h"
#include "_source.h"
// ***** Start of method declarations.
// ***** End of method declarations.
#include "include/api/reaction_macros.h"
void _sourcereaction_function_0(void* instance_args) {
    _source_self_t* self = (_source_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _source_y_t* y = &self->_lf_y;
    #line 27 "/home/u-tajima/lf-test/src/DeadlineTest2.lf"
    long logical_fire_ns = lf_time_logical(); // logical time in ns (API依存、または自分で定義)
    long reaction_start_ns = get_time_ns();
    
    long scheduler_latency_ns = reaction_start_ns - logical_fire_ns;
    printf("Scheduler latency: %ld ns\n", scheduler_latency_ns);
    
    // CPU負荷をかけるダミー計算
    volatile double x_dummy = 0;
    for (int i = 0; i < DUMMY_LOOP_COUNT; i++) {  // ← 回数は調整可能
        x_dummy += i * 0.0001;
    }
    
    long send_time_ns = get_time_ns();
    
    // 周期ジッタ出力
    if (self->last_send_time >= 0) {
        long interval_ns = send_time_ns - self->last_send_time;
        printf("[%ld.%09ld] Source sends: %d at %ld ns (interval: %ld ns)\n", 
            send_time_ns / 1000000000L, send_time_ns % 1000000000L, self->count, send_time_ns, interval_ns);
    } else {
        printf("[%ld.%09ld] Source sends: %d at %ld ns (first send)\n", 
            send_time_ns / 1000000000L, send_time_ns % 1000000000L, self->count, send_time_ns);
    }
    
    self->last_send_time = send_time_ns;
    
    lf_set(y, send_time_ns);
    (self->count)++;
#line 44 "/home/u-tajima/lf-test/src-gen/DeadlineTest2/_source.c"
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
