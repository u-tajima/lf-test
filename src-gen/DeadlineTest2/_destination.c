#include "include/api/schedule.h"
#ifdef FEDERATED
#include <string.h>
#endif // FEDERATED
#include "low_level_platform/api/low_level_platform.h"
#include "include/DeadlineTest2/Destination.h"
#include "_destination.h"
// ***** Start of method declarations.
// ***** End of method declarations.
#include "include/api/reaction_macros.h"
void _destinationreaction_function_0(void* instance_args) {
    _destination_self_t* self = (_destination_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _destination_x_t* x = self->_lf_x;
    int x_width = self->_lf_x_width; SUPPRESS_UNUSED_WARNING(x_width);
    #line 67 "/home/u-tajima/lf-test/src/DeadlineTest2.lf"
    (self->count)++;
    (self->total_received)++;
    
    long recv_time_ns = get_time_ns();
    long send_time_ns = x->value;
    long error_ns = recv_time_ns - send_time_ns;
    
    // 必要ならここにも負荷を入れる
    volatile double x_dummy = 0;
    for (int i = 0; i < DUMMY_LOOP_COUNT; i++) {  
        x_dummy += i * 0.0001;
    }
    
    // Destination latency出力
    printf("[%ld.%09ld] Destination receives msg, send_time: %ld ns, recv_time: %ld ns, error: %ld ns\n", 
        recv_time_ns / 1000000000L, recv_time_ns % 1000000000L,
        send_time_ns, recv_time_ns, error_ns);
    
    if (self->total_received >= self->max_count) {
        printf("=== Summary ===\n");
        printf("Total received: %d\n", self->total_received);
        printf("Deadline misses: %d\n", self->deadline_miss_count);
        exit(0);
    }
#line 41 "/home/u-tajima/lf-test/src-gen/DeadlineTest2/_destination.c"
}
void _destination_deadline_function0(void* instance_args) {
    _destination_self_t* self = (_destination_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _destination_x_t* x = self->_lf_x;
    int x_width = self->_lf_x_width; SUPPRESS_UNUSED_WARNING(x_width);
    #line 94 "/home/u-tajima/lf-test/src/DeadlineTest2.lf"
    (self->deadline_miss_count)++;
    (self->count)++;
    (self->total_received)++;
    
    long recv_time_ns = get_time_ns();
    long send_time_ns = x->value;
    long error_ns = recv_time_ns - send_time_ns;
    
    // Deadline miss時のlog
    printf("[%ld.%09ld] Destination receives msg, send_time: %ld ns, recv_time: %ld ns, error: %ld ns --Deadline misses--\n", 
        recv_time_ns / 1000000000L, recv_time_ns % 1000000000L,
        send_time_ns, recv_time_ns, error_ns);
    
    if (self->total_received >= self->max_count) {
        printf("=== Summary ===\n");
        printf("Total received: %d\n", self->total_received);
        printf("Deadline misses: %d\n", self->deadline_miss_count);
        exit(0);
    }
#line 67 "/home/u-tajima/lf-test/src-gen/DeadlineTest2/_destination.c"
}
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
