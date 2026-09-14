#include "logger.h"

#if defined(LH_LOG_TO_CONSOLE)
#include <extdll.h>
#include "sdk_util.h"
#else
#include <ctime>

#include <fcntl.h>
#include <unistd.h>
#endif

#include <cstdarg>
#include <cstdio>

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

    int fd = open(
        "last_hope.log", 
        O_WRONLY | O_CREAT | O_APPEND,
        0644
    );

    if (fd < 0)
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
        write(fd, output, lenght);

    close(fd);
#endif
}
