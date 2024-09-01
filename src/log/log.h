#ifndef LOG_INCLUDE_GUARD
#define LOG_INCLUDE_GUARD

#include <stdio.h>
#include <stdarg.h>

#ifdef LOG_FILE
#include <stdint.h>
#include <string.h>
#define BUFFER_LENGTH 256
#endif

#if defined LOG || defined LOG_FILE
    #define INFO(...) log_info(__FILE__, __LINE__, __VA_ARGS__)
    #define DBG(...) log_debug(__FILE__, __LINE__, __VA_ARGS__)
    #define WARNING(...) log_warning(__FILE__, __LINE__, __VA_ARGS__)
    #define ERROR(...) log_error(__FILE__, __LINE__, __VA_ARGS__)
#else
    #define DBG(...) do {} while (0)
    #define INFO(...)  do {} while (0)
    #define WARNING(...) do {} while (0)
    #define ERROR(...)  do {} while (0)
#endif


void log_info(const char* file, int line, const char* fmt, ...);
void log_debug(const char* file, int line, const char* fmt, ...);
void log_warning(const char* file, int line, const char* fmt, ...);
void log_error(const char* file, int line, const char* fmt, ...);

#ifdef LOG_FILE
void log_file(const char* file, int line, const char* type, const char* fmt, va_list* args);
#endif


#endif
