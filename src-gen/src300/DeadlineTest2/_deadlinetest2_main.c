#include "include/api/schedule.h"
#include <string.h>
#include "low_level_platform/api/low_level_platform.h"
#include "include/DeadlineTest2/DeadlineTest2.h"
#include "_deadlinetest2_main.h"
// *********** From the preamble, verbatim:
#line 187 "/home/u-tajima/lf-test/src300/DeadlineTest2.lf"
SourceLog s_logs[MAX_SAMPLES];
DestLog d_logs[MAX_SAMPLES];
int s_log_idx = 0;
int d_log_idx = 0;
#line 13 "/home/u-tajima/lf-test/src-gen/src300/DeadlineTest2/_deadlinetest2_main.c"

// *********** End of preamble.
// ***** Start of method declarations.
// ***** End of method declarations.
_deadlinetest2_main_main_self_t* new__deadlinetest2_main() {
    _deadlinetest2_main_main_self_t* self = (_deadlinetest2_main_main_self_t*)lf_new_reactor(sizeof(_deadlinetest2_main_main_self_t));

    return self;
}
