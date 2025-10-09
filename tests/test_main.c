#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#define C_LOG_IMPLEMENTATION
#include "c_log.h"

extern const struct CMUnitTest c_log_tests[];
extern const size_t c_log_tests_count;

int main(void) {
    int fail = 0;

    // Run the logger test group
    fail += _cmocka_run_group_tests("c_log.h tests", c_log_tests, c_log_tests_count, NULL, NULL);

    return fail;
}
