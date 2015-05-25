#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include <sys/types.h>
#include <unistd.h>

#define BASE_CMD_LENGTH 16 // "ionice -c 3 -p "

PG_MODULE_MAGIC;

Datum pg_nice(PG_FUNCTION_ARGS);
static size_t width_as_string(pid_t pid);

PG_FUNCTION_INFO_V1(pg_nice);

inline size_t
width_as_string(pid_t pid)
{
    size_t count = 0;
    while (pid != 0)
    {
        pid /= 10;
        ++count;
    }
    return count;
}

Datum
pg_nice(PG_FUNCTION_ARGS)
{
    int32 result;
    pid_t pid = getpid();
    char *cmd = palloc0(BASE_CMD_LENGTH + width_as_string(pid));
    sprintf(cmd, "ionice -c 3 -p %d", pid);
    result = system(cmd);
    PG_RETURN_INT32(result);
}
