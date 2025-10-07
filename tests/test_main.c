#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#define C_LOG_IMPLEMENTATION
#include "c_log.h"

extern const struct CMUnitTest logger_tests[];
extern const size_t logger_tests_count;

int main(void) {
    int fail = 0;

    // Run the logger test group
    fail += _cmocka_run_group_tests("Logger tests", logger_tests, logger_tests_count, NULL, NULL);

    return fail;
}
