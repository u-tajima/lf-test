#include "include/api/schedule.h"
#ifdef FEDERATED
#include <string.h>
#endif // FEDERATED
#include "low_level_platform/api/low_level_platform.h"
#include "include/DeadlineTest2/DeadlineTest2.h"
#include "_deadlinetest2_main.h"
// ***** Start of method declarations.
// ***** End of method declarations.
_deadlinetest2_main_main_self_t* new__deadlinetest2_main() {
    _deadlinetest2_main_main_self_t* self = (_deadlinetest2_main_main_self_t*)lf_new_reactor(sizeof(_deadlinetest2_main_main_self_t));

    return self;
}
