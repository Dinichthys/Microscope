#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "logging.hpp"

static enum LevelLog sLoggingLevel = kWarning;
static FILE* sLoggingStream        = stderr;
static const int kYearShift        = 1900;
static const int kMonthShift       = 1;

static void LevelToStr(const enum LevelLog level);

static void LevelToStr(const enum LevelLog level) {
    switch (level) {
        case kDebug: {
            fprintf(sLoggingStream, "[%s] \n", "DEBUG");
            break;
        }
        case kInfo: {
            fprintf(sLoggingStream, "[%s] \n", "INFO");
            break;
        }
        case kWarning: {
            fprintf(sLoggingStream, "[%s] \n", "WARNING");
            break;
        }
        case kError: {
            fprintf(sLoggingStream, "[%s] \n", "ERROR");
            break;
        }
        default: {
            assert(0 && "Program got wrong level of logging in function log\n");
            break;
        }
    }
}


void Log(const struct logging settings, const enum LevelLog level, const char * const format, ...) {
    assert((format != NULL) && "Invalid argument format for Log\n");

    if (level < sLoggingLevel) {
        return;
    }

    LevelToStr(level);
    fprintf(sLoggingStream, "%s:%d (%s) %d sec %d min %d hours %d days %d month %d year\n",
            settings.file, settings.line, settings.func,
            settings.now->tm_sec, settings.now->tm_min,
            settings.now->tm_hour, settings.now->tm_mday,
            settings.now->tm_mon + kMonthShift, settings.now->tm_year + kYearShift);

    va_list param;
    va_start(param, format);
    vfprintf(sLoggingStream, format, param);
    va_end(param);

    fprintf(sLoggingStream, "\n\n");
}

void SetLogLevel(const enum LevelLog level) {
    sLoggingLevel = level;
}

void SetLogFile(FILE* const file) {
    if (file != NULL) {
        sLoggingStream = file;
        setbuf(file, NULL);
    }
}
