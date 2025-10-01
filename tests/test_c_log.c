#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#define C_LOG_LEVEL 4
#include "c_log.h"

#define BUFFER_SIZE 512

// Helper to read the log file into a buffer
static void read_log_file(char *buffer, size_t bufsize)
{
    FILE *f = fopen("./cvk.log", "r");
    assert_non_null(f);
    size_t n = fread(buffer, 1, bufsize - 1, f);
    buffer[n] = 0;
    fclose(f);
}

static int setup(void **state)
{
    // UNUSED
    (void)state;

    c_log_open("./cvk.log");

    return 0;
}

static int teardown(void **state)
{
    // UNUSED
    (void)state;

    c_log_close();
    remove("./cvk.log");

    return 0;
}

// Test that the log file has been created (in setup)
static void test_log_file_creation(void **state)
{
    // UNUSED
    (void)state;

    FILE *f = fopen("./cvk.log", "r");
    assert_non_null(f);
    fclose(f);
}

static void test_log_message_written(void **state)
{
    // UNUSED
    (void)state;

    C_LOG_ERROR("Test message: %d", 42);
    c_log_close();

    char buf[BUFFER_SIZE];
    read_log_file(buf, sizeof(buf));
    assert_true(strstr(buf, "Test message: 42") != NULL);
    assert_true(strstr(buf, "[ERR] ") != NULL);
}

static void test_log_level_color_prefix(void **state)
{
    // UNUSED
    (void)state;

    C_LOG_ERROR("Err!");
    C_LOG_WARN("Warn!");
    C_LOG_INFO("Info!");
    C_LOG_DEBUG("Debug!");
    C_LOG_TRACE("Trace!");
    c_log_close();

    char buf[BUFFER_SIZE];
    read_log_file(buf, sizeof(buf));
    assert_true(strstr(buf, "[ERR] ") != NULL);
    assert_true(strstr(buf, "[WRN] ") != NULL);
    assert_true(strstr(buf, "[INF] ") != NULL);
    assert_true(strstr(buf, "[DBG] ") != NULL);
    assert_true(strstr(buf, "[TRC] ") != NULL);
}

const struct CMUnitTest logger_tests[] = {
    cmocka_unit_test_setup_teardown(test_log_file_creation, setup, teardown),
    cmocka_unit_test_setup_teardown(test_log_message_written, setup, teardown),
    cmocka_unit_test_setup_teardown(test_log_level_color_prefix, setup, teardown),
};

const size_t logger_tests_count = sizeof(logger_tests) / sizeof(logger_tests[0]);
