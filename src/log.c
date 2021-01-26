#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#include "fsh.h"

char *_fsh_get_loglevel_id() {
    switch(fsh_loglevel) {
        case LOGLEVEL_ERROR:
            return "[ERR]";
        case LOGLEVEL_WARNING:
            return "[WARN]";
        case LOGLEVEL_LOG:
            return "[LOG]";
        default:
            return "[fsh]";
    }
}

void fsh_log(enum LOGLEVEL msg_type, const char *msg) {
    if (msg_type <= fsh_loglevel && fsh_log_file) {
        time_t t = time(NULL);
        struct tm *tms = localtime(&t);

        char *strtime = strtok(asctime(tms), "\n");



        fprintf(fsh_log_file, "%s %s %s\n", strtime, _fsh_get_loglevel_id(), msg);
        fflush(fsh_log_file);
    }
}

void fsh_logf(enum LOGLEVEL msg_type, const char *fmt, ...) {
    if (msg_type <= fsh_loglevel && fsh_log_file) {
        time_t t = time(NULL);
        struct tm *tms = localtime(&t);

        char *strtime = strtok(asctime(tms), "\n");

        char *buf = (char*) malloc(2048);

        va_list arg;
        va_start(arg, fmt);

        vsprintf(buf, fmt, arg);

        va_end(arg);

        fprintf(fsh_log_file, "%s %s %s\n", strtime, _fsh_get_loglevel_id(), buf);
        fflush(fsh_log_file);
    }
}
