#include "logger.h"

#if defined(LH_LOG_TO_CONSOLE)
#include <extdll.h>
#include "sdk_util.h"
#else
#include <cerrno>
#include <sys/stat.h>
#include <ctime>

#include <fcntl.h>
#include <unistd.h>

static int g_log_fd = -1;

#endif

#include <cstdarg>
#include <cstdio>
#include <cstring>

#if !defined(LH_LOG_TO_CONSOLE)

void LH_LogInit()
{
    if (mkdir("last_hope_logs", 0755) < 0 && errno != EEXIST)
    {
        fprintf(
            stderr,
            "[LastHope] mkdir failed: %s\n",
            strerror(errno)
        );
        return;
    }

    g_log_fd = open(
        "last_hope_logs/last_hope.log", 
        O_WRONLY | O_CREAT | O_APPEND,
        0644
    );

    if (g_log_fd < 0)
    {
        fprintf(
            stderr,
            "[LastHope] open log failed: %s\n",
            strerror(errno)
        );
        return;
    }
}

void LH_LogShutdown()
{
    if (g_log_fd >= 0)
    {
        close(g_log_fd);
        g_log_fd = -1;
    }
}

#else 

void LH_LogInit() {}
void LH_LogShutdown() {}

#endif

void LH_Log(const char *level, const char *format, ...)
{
    char message[1024];

    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

#if defined (LH_LOG_TO_CONSOLE)

    UTIL_LogPrintf(
        "[LastHope] [%s] %s\n",
        level,
        message
    );

#else
    if (g_log_fd < 0)
        return;

    time_t now = time(nullptr);
    tm local_time{};

    localtime_r(&now, &local_time);

    char timestamp[32];

    strftime(
        timestamp, 
        sizeof(timestamp),
        "%H:%M:%S",
        &local_time
    );
    
    char output[1200];

    int lenght = snprintf(
        output,
        sizeof(output),
        "[%s] [%s] %s\n",
        timestamp,
        level,
        message
    );
    
    if (lenght > 0)
        write(g_log_fd, output, lenght);
#endif
}
