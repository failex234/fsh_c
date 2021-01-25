#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "fsh.h"


int fsh_printf(const char *fmt, ...) {
    int ret;
    va_list args;
    va_start(args, fmt);

    ret = vfprintf(stdout, fmt, args);

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

void fsh_read_config() {
    char *home = get_home_dir();
    size_t homelen = strlen(home);
    char *configpath = (char*) malloc(homelen + 17);
    FILE *conf;

    sprintf(configpath, "%s/.config/fsh.cfg", home);

    conf = fopen(configpath, "r");
    fsh_config = (Config*) malloc(sizeof(Config));

    if (!conf) {
        fsh_logf(LOGLEVEL_LOG, "config file %s doens't exist, loading default config\n", configpath);

        char *syspath = getenv("PATH");

        if (!syspath) {
            //Fall back to known paths
            fsh_config->path = (char*) malloc(30);
            strcpy(fsh_config->path, "/bin:/usr/bin:/sbin:/usr/sbin");
        } else {
            fsh_config->path = (char *) malloc(strlen(syspath) + 1);
            strcpy(fsh_config->path, syspath);
        }

        fsh_config->ps1 = (char*) malloc(14);
        set_bg_color(BLACK);
        set_fg_color(WHITE);



        strcpy(fsh_config->ps1, "[\\u@\\h \\w]\\$ ");
    }/* else {
        fsh_logf(LOGLEVEL_LOG, "config file %s found, loading contents\n", configpath);

        char **contents = key_and_val_split_file(conf, '=');

        while (*contents) {
            char *key = *contents;
            contents++;
            char *value = *contents;
            contents++;

            if (!strcmp(key, "PATH")) {
                fsh_config->path = malloc(strlen(value) + 1);
                strcpy(fsh_config->path, value);
            } else if (!strcmp(key, "PS1")) {
                fsh_config->ps1 = malloc(strlen(value) + 1);
                strcpy(fsh_config->ps1, value);
            }
        }

        char **contents_ptr = contents;

        while(*contents) {
            free(*contents);
            contents++;
        }

        free(contents_ptr);
        fclose(conf);
    } */


}
/*
char **key_and_val_split_file(FILE *file, char seperator) {

}
*/
