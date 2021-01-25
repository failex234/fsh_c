//
// Created by Felix Naumann on 25.01.21.
//
#include "fsh.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

char *get_fsh_path() {
    char *home = get_home_dir();
    size_t home_len = strlen(home);
    char *full_path = (char*) malloc(home_len + 14);
    sprintf(full_path, "%s/.config/fsh", home);

    return full_path;
}

char *get_config_folder() {
    char *home = get_home_dir();
    size_t home_len = strlen(home);

    char *full_path = (char*) malloc(home_len + 9);
    sprintf(full_path, "%s/.config", home);

    return full_path;
}

char *get_fsh_file_or_directory(const char *name) {
    char *fsh_path = get_fsh_path();

    size_t fshpath_len = strlen(fsh_path);
    size_t name_len = strlen(name);

    char *full_path = (char *) malloc(fshpath_len + name_len + 1);
    sprintf(full_path, "%s/%s", fsh_path, name);

    return full_path;
}

FILE *get_fsh_file(const char *name, const char *mode) {
    char *fsh_path = get_fsh_path();

    size_t fsh_path_len = strlen(fsh_path);
    size_t file_name_len = strlen(name);

    char *fullpath = (char*) malloc(fsh_path_len + file_name_len + 2);
    sprintf(fullpath, "%s/%s", fsh_path, name);

    FILE *retfile = fopen(fullpath, mode);
    free(fullpath);

    return retfile;
}


void openlog() {
    fsh_log_file = get_fsh_file("logs/general.log", "w");
}

void make_config_folder() {
    struct stat dirst = {0};

    char *fsh_folder = get_fsh_path();
    char *fsh_log_folder = get_fsh_file_or_directory("logs");
    char *config_folder = get_config_folder();

    if (stat(config_folder, &dirst) == -1) {
        mkdir(config_folder, 0700);
    }

    if (stat(fsh_folder, &dirst) == -1) {
        mkdir(fsh_folder, 0700);
    }

    if (stat(fsh_log_folder, &dirst) == -1) {
        mkdir(fsh_log_folder, 0700);
    }
}