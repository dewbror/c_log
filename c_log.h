/*
c_log.h - v0.1.0

Documentation:
    TODO: Write this...
*/

#ifndef C_LOG_H_
#define C_LOG_H_

#include <stdint.h> // Needed for fixed-width types

#ifdef __cplusplus
extern "C" {
#endif

#define C_LOG_LEVEL_ERROR 0
#define C_LOG_LEVEL_WARN  1
#define C_LOG_LEVEL_INFO  2
#define C_LOG_LEVEL_DEBUG 3
#define C_LOG_LEVEL_TRACE 4

/*
Open file 'file_name' for logging, 'file_name' == NULL will set logging to stderr.

\param[in] file_name A string with the name of the log file to open.
*/
void c_log_open(const char *file_name);

/*
Close log file.
*/
void c_log_close(void);

// Check for GCC or Clang
#if defined(__GNUC__) || defined(__clang__)
#define FORMAT_ATTR(format_index, first_arg_index) __attribute__((format(printf, format_index, first_arg_index)))
#else
#define FORMAT_ATTR(format_index, first_arg_index) // NOP for MSVC and other compilers
#endif

/*
NOT MEANT TO BE USED! Use C_LOG_ERROR, _INFO, ... etc. instead.

Valid C_LOG_LEVELS are:
    0: ERROR, 
    1: WARNING, 
    2: INFO, 
    3: DEBUG, 
    4 to UINT8_MAX: TRACE. 

\param[in] level The C_LOG_LEVEL of the message, 
\param[in] fmt A printf-style message format string.
\param[in] ... Additional parameters matching % tokens in the "fmt" string, if any.
*/
void __c_log__msg(const uint8_t level, const char *const file, const int32_t line, const char *const func,
    const char *const fmt, ...) FORMAT_ATTR(5, 6);

#undef FORMAT_ATTR

// If C_LOG_LEVEL is not defined, use default C_LOG_LEVEL
#ifndef C_LOG_LEVEL
#ifdef NDEBUG
// Default C_LOG_LEVEL for NDEBUG mode is C_LOG_INFO
#define C_LOG_LEVEL C_LOG_LEVEL_INFO
#else
// Default C_LOG_LEVEL in debug mode is C_LOG_TRACE
#define C_LOG_LEVEL C_LOG_LEVEL_TRACE
#endif
#endif

#if C_LOG_LEVEL >= C_LOG_LEVEL_ERROR
#define C_LOG_ERROR(...) __c_log__msg(C_LOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define C_LOG_ERROR(...) ((void)0)
#endif

#if C_LOG_LEVEL >= C_LOG_LEVEL_WARN
#define C_LOG_WARN(...) __c_log__msg(C_LOG_LEVEL_WARN, __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define C_LOG_WARN(...) ((void)0)
#endif

#if C_LOG_LEVEL >= C_LOG_LEVEL_INFO
#define C_LOG_INFO(...) __c_log__msg(C_LOG_LEVEL_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define C_LOG_INFO(...) ((void)0)
#endif

#if C_LOG_LEVEL >= C_LOG_LEVEL_DEBUG
#define C_LOG_DEBUG(...) __c_log__msg(C_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define C_LOG_DEBUG(...) ((void)0)
#endif

#if C_LOG_LEVEL >= C_LOG_LEVEL_TRACE
#define C_LOG_TRACE(...) __c_log__msg(C_LOG_LEVEL_TRACE, __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define C_LOG_TRACE(...) ((void)0)
#endif

#ifdef __cplusplus
}
#endif

#ifdef C_LOG_IMPLEMENTATION

#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define TIMEBUF_MAX 32
#define INFOBUF_MAX 256

#define COLOR_RED    "\x1b[31m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_GREEN  "\x1b[32m"
#define COLOR_BLUE   "\x1b[34m"
#define COLOR_CYAN   "\x1b[36m"
#define COLOR_RESET  "\x1b[0m"

// Global states NOT thread safe!!
static bool logging_to_file = true;
static FILE *fp_log = NULL;
static const char *log_file_name = NULL;

void c_log_open(const char *const file_name)
{
    // If file_name == NULL, set logging to stderr
    if(file_name == NULL) {
        fp_log = stderr;
        logging_to_file = false;
        return;
    }

    // If file_name != NULL, open it as logging file
    fp_log = fopen(file_name, "w");
    if(fp_log == NULL)
        return;

    // Set the name of the log file
    log_file_name = file_name;
    logging_to_file = true;

    C_LOG_INFO("Log file opened '%s'", log_file_name);
}

void c_log_close(void)
{
    if(!logging_to_file)
        return;

    C_LOG_DEBUG("Attempting to close log file '%s'", log_file_name);

    // Check if log_file is NULL
    if(fp_log != NULL) {
        // Close log_file
        int ret = fclose(fp_log);
        if(ret != 0)
            return;

        // Set file ptr to default stderr
        fp_log = NULL;
        logging_to_file = false;
    }
}

void __c_log__msg(const uint8_t level, const char *const file, const int32_t line, const char *const func,
    const char *const fmt, ...)
{
    // UNUSED
    (void)file;
    (void)line;

    int ret = 0;
    size_t ret_LU = 0;
    const char *color = NULL;

    // Set the color based on the log level of the message
    switch(level) {
    case C_LOG_LEVEL_ERROR:
        color = COLOR_RED;
        break;
    case C_LOG_LEVEL_WARN:
        color = COLOR_YELLOW;
        break;
    case C_LOG_LEVEL_INFO:
        color = COLOR_GREEN;
        break;
    case C_LOG_LEVEL_DEBUG:
        color = COLOR_BLUE;
        break;
    case C_LOG_LEVEL_TRACE:
        color = COLOR_CYAN;
        break;
    default:
        color = COLOR_RESET;
        break;
    }

    // Get the current time
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);

    // Format time string
    char timebuf[TIMEBUF_MAX];
    ret_LU = strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm_now);
    if(ret_LU == 0)
        return;

    char infobuf[INFOBUF_MAX];
    // ret = snprintf(infobuf, sizeof(infobuf), "%s:%d [%s]", file, line, func);
    ret = snprintf(infobuf, sizeof(infobuf), "[%s]", func);
    // TODO: check if ret < INFOBUF_MAX, handle buffer to small error
    if(ret == 0)
        return;

    if(fp_log == NULL) {
        // If fp_log == NULL print an error directly to stderr.
        // fprintf(stderr, "%s %s[ERROR]%s fp_log == NULL in logger.c\n", timebuf, COLOR_RED, COLOR_RESET);
        return;
    }

    // Print date and time
    static const char *const levels[] = {"[ERR] ", "[WRN] ", "[INF] ", "[DBG] ", "[TRC] "};
    if(logging_to_file) {
        ret = fprintf(fp_log, "%s %s", timebuf, levels[level]);
        if(ret < 0)
            return;
    }
    else {
        ret = fprintf(fp_log, "%s %s%s%s%s ", timebuf, color, levels[level], COLOR_RESET, infobuf);
        if(ret < 0)
            return;
    }

    if(fmt != NULL) {
        // Initialize variable argument list
        va_list args;
        va_start(args, fmt);

        // Print formatted string
        ret = vfprintf(fp_log, fmt, args);
        if(ret < 0) {
            va_end(args);
            return;
        }

        // Cleanup va lists
        va_end(args);
    }

    // Print newline
    ret = fprintf(fp_log, "\n");
    if(ret < 0)
        return;
}

// Undef macros
#undef TIMEBUF_MAX
#undef INFOBUF_MAX

#undef COLOR_RED
#undef COLOR_YELLOW
#undef COLOR_GREEN
#undef COLOR_CYAN
#undef COLOR_RESET

#endif // C_LOG_IMPLEMENTATION

#endif // C_LOG_H_

/*
Version History:
    0.1.0 (2025-10-01) First released version.
*/

/*
Copyright (c) 2025 dewbror <dewbror@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the “Software”), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.
*/
