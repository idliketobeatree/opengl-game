#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define LOG_USE_COLOR

#define LOG_DEBUG
#define LOG_INFO
#define LOG_WARN
#define LOG_ERROR

#define DOUBLE_SEPARATOR   "================================================================================"
#define STRAIGHT_SEPARATOR "----------------------------------------------------------------"
#define WIGGLY_SEPARATOR   "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

// #define debug
// #define info
// #define warn
// #define error

struct Logger {
    const char *name;
    const char *color;
    const signed char level;
};
struct LogEvent {
    Logger *logger;
    va_list ap;
    const char *fmt;
    const char *file;
    struct tm *time;
    FILE *udata;
    int line;
};


static signed char level;

const static char *FORMAT = "[%s] %s\n";

static inline void initEvent(LogEvent *ev, FILE *udata) {
    if (!ev->time) {
        time_t t = time(NULL);
        ev->time = localtime(&t);
    }
    ev->udata = udata;
}

static inline void stdoutCallback(LogEvent *ev) {
    char buf[16];
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
#ifdef LOG_USE_COLOR
    fprintf(
            ev->udata, "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
            buf, ev->logger->color, ev->logger->name,
            ev->file, ev->line);
#else
    fprintf(
            ev->udata, "%s %-5s %s:%d: ",
            buf, ev->logger->name, ev->file, ev->line);
#endif
    vfprintf(ev->udata, ev->fmt, ev->ap);
    fprintf(ev->udata, "\n");
    fflush(ev->udata);
}

const inline void log(Logger logger, const char *file, int line, const char *fmt, ...) {
    LogEvent ev{
            .logger = &logger,
            .fmt    = fmt,
            .file   = file,
            .line   = line,
    };
    if (logger.level > level) {
        initEvent(&ev, stdout);
        va_start(ev.ap, fmt);
        stdoutCallback(&ev);
        va_end(ev.ap);
    }
}

const extern Logger FATAL;
#define fatal(...) log(FATAL, __FILE__, __LINE__, __VA_ARGS__)

#ifdef LOG_DEBUG
const extern Logger DEBUG;
#define debug(...) log(DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#endif

#ifdef LOG_INFO
const extern Logger INFO;
#define info(...) log(INFO, __FILE__, __LINE__, __VA_ARGS__)
#define doubleSeparator() log(INFO, __FILE__, __LINE__, DOUBLE_SEPARATOR)
#define straightSeparator() log(INFO, __FILE__, __LINE__, STRAIGHT_SEPARATOR)
#define wigglySeparator() log(INFO, __FILE__, __LINE__, WIGGLY_SEPARATOR)
#define line() log(INFO, __FILE__, __LINE__, "")
#endif

#ifdef LOG_WARN
const extern Logger WARN;
#define warn(...) log(WARN, __FILE__, __LINE__, __VA_ARGS__)
#endif

#ifdef LOG_ERROR
const extern Logger ERROR;
#define error(...) log(ERROR, __FILE__, __LINE__, __VA_ARGS__)
#endif

inline void changeLevel(signed char logLevel) {
#ifdef LOG_DEBUG
    level = -1;
    debug("Log level changed to %d", logLevel);
#endif
    level = logLevel;
}
