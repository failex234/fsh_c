#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "fsh.h"


int fsh_printf(const char *fmt, ...) {
    char *colors = get_single_color(0);

    size_t colorslen = strlen(colors);
    size_t fmtlen = strlen(fmt);

    char *extendedfmt = (char *) malloc(colorslen + fmtlen + 1);

    memcpy(extendedfmt, colors, colorslen);
    memcpy(extendedfmt + colorslen, fmt, fmtlen);

    extendedfmt[colorslen + fmtlen] = '\0';

    int ret;
    va_list args;
    va_start(args, extendedfmt);

    ret = vfprintf(stdout, extendedfmt, args);

    va_end(args);

    return ret;
}


int fsh_printerr(const char *fmt, ...) {
    int ret;
    va_list args;

    va_start(args, fmt);

    ret = vfprintf(stderr, fmt, args);

    va_end(args);
    return ret;
}

/*
char **key_and_val_split_file(FILE *file, char seperator) {

}
*/
