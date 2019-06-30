#pragma once

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

enum mcpelauncher_log_level {
    MCPELAUNCHER_LOG_TRACE,
    MCPELAUNCHER_LOG_DEBUG,
    MCPELAUNCHER_LOG_INFO,
    MCPELAUNCHER_LOG_WARN,
    MCPELAUNCHER_LOG_ERROR
};

void *mcpelauncher_hook(void *symbol, void *hook, void **original);

void mcpelauncher_vlog(mcpelauncher_log_level level, const char *tag, const char *text, va_list args);
void mcpelauncher_log(mcpelauncher_log_level level, const char *tag, const char *text, ...);

#ifdef __cplusplus
}
#endif
