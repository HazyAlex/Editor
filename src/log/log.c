#include "log.h"


const char* skip_src(const char* file)
{
    while (*file != '\0' && *file != '/') {
        ++file;
    }

    return *file == '\0' ? file : ++file;
}

void log_info(const char* file, int line, const char* fmt, ...)
{
#if !defined(LOG) && !defined(LOG_FILE)
    (void)file;
    (void)line;
    return;
#endif

    va_list args;
    va_start(args, fmt);

#ifdef LOG
    fprintf(stderr, "[INFO  %s:%d]: ", skip_src(file), line);
    vfprintf(stderr, fmt, args);
#endif
#ifdef LOG_FILE
    log_file(file, line, "INFO ", fmt, &args);
#endif

    va_end(args);
}

void log_debug(const char* file, int line, const char* fmt, ...)
{
#if !defined(LOG) && !defined(LOG_FILE)
    (void)file;
    (void)line;
    return;
#endif

    va_list args;
    va_start(args, fmt);

#ifdef LOG
    fprintf(stderr, "[DEBUG %s:%d]: ", skip_src(file), line);
    vfprintf(stderr, fmt, args);
#endif
#ifdef LOG_FILE
    log_file(file, line, "DEBUG", fmt, &args);
#endif

    va_end(args);
}

void log_warning(const char* file, int line, const char* fmt, ...)
{
#if !defined(LOG) && !defined(LOG_FILE)
    (void)file;
    (void)line;
    return;
#endif

    va_list args;
    va_start(args, fmt);

#ifdef LOG
    fprintf(stderr, "[WARNING %s:%d]: ", skip_src(file), line);
    vfprintf(stderr, fmt, args);
#endif
#ifdef LOG_FILE
    log_file(file, line, "WARNING", fmt, &args);
#endif

    va_end(args);
}

void log_error(const char* file, int line, const char* fmt, ...)
{
#if !defined(LOG) && !defined(LOG_FILE)
    (void)file;
    (void)line;
    return;
#endif

    va_list args;
    va_start(args, fmt);

#ifdef LOG
    fprintf(stderr, "[ERROR %s:%d]: ", skip_src(file), line);
    vfprintf(stderr, fmt, args);
#endif
#ifdef LOG_FILE
    log_file(file, line, "ERROR", fmt, &args);
#endif

    va_end(args);
}

#ifdef LOG_FILE
void log_file(const char* file, int line, const char* type, const char* fmt, va_list* args)
{
    FILE* logFile = fopen("output.log", "a");

    char buffer[BUFFER_LENGTH];
    size_t wroteBytes = (size_t)snprintf(buffer, BUFFER_LENGTH, "[%s %s:%d]: ", type, skip_src(file), line);
    fwrite(buffer, sizeof(char), wroteBytes, logFile);
    memset(buffer, '\0', BUFFER_LENGTH);

    wroteBytes = (size_t)vsnprintf(buffer, BUFFER_LENGTH, fmt, *args);
    if (wroteBytes >= BUFFER_LENGTH - 1) {
        wroteBytes = BUFFER_LENGTH - 1; // TRUNCATED, vsnprintf should '\0' the buffer
    }
    fwrite(buffer, sizeof(char), wroteBytes, logFile);

    fclose(logFile);
}
#endif
