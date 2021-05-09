#include "logging.hpp"

#include <string.h>

#define RESET "\x1b[0m"

#define CYAN_FOREGROUND "\x1b[96m"
#define RED_FOREGROUND "\x1b[91m"
#define DARK_RED_FOREGROUND "\x1b[31m"

#define RED_FOREGROUND "\x1b[91m"

#define DEBUG_COLOR CYAN_FOREGROUND
#define INFO_COLOR CYAN_FOREGROUND
#define WARN_COLOR RED_FOREGROUND
#define ERROR_COLOR RED_FOREGROUND
#define FATAL_COLOR DARK_RED_FOREGROUND

const Logger FATAL = {"[ FATAL ]", FATAL_COLOR, 127};


#ifdef LOG_DEBUG
const Logger DEBUG = {"[ DEBUG ]", DEBUG_COLOR, -1};
#endif

#ifdef LOG_INFO
const Logger INFO = {"[ INFO  ]", INFO_COLOR, 0};
#endif

#ifdef LOG_WARN
const Logger WARN = {"[WARNING]", WARN_COLOR, 1};
#endif

#ifdef LOG_ERROR
const Logger ERROR = {"[ ERROR ]", ERROR_COLOR, 2};
#endif