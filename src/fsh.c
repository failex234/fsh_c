#include <stdlib.h>
#include <getopt.h>

#include "fsh.h"

static struct option long_options[] = {
    {"help",            no_argument,    0, 'h'},
    {"version",         no_argument,    0, 'v'},
    {"verbose",         no_argument,    0, 'V'},
    {"sverbose",    no_argument,    0, 'S'},

    {0, 0, 0, 0}
};

void print_help(char *prgname) {
    printf(""
            "usage: %s [arguments]"
            "\n"
            "  options:\n"
            "    --help,\t -h - Show this menu\n"
            "    --version,\t -v - Show version information\n"
            "    --verbose,\t -V - activate verbose (warning log) mode\n"
            "    --sverbose,\t -S - activate super verbose mode (all log messages)\n"
            , prgname);
};

void print_version(char *prgname) {
    printf("%s\n", FSH_VERSION);
}

void fsh_loop() {
    char *line, **args;
    int status;

    do {
        char *proc_ps1 = parse_ps1();
        printf("%s", proc_ps1);

        //Get input and split it correctly so that we can
        //parse and interpret it
        line = read_line();
        args = split_line(line);
        status = execute(args);

        free(line);
        char **args_ptr = args;

        while(*args) {
            free(*args);
            args++;
        }
        free(args_ptr);
        free(proc_ps1);

    } while (status);
}


int main(int argc, char **argv) {
    fsh_loglevel = LOGLEVEL_ERROR;

    int c = 0;
    while (1) {
        c = getopt_long(argc, argv, "hvVS", long_options, NULL);

        if (c == -1) break;

        switch(c) {
            case 'h':
                print_help(argv[0]);
                exit(0);
            case 'v':
                print_version(argv[0]);
                exit(0);
            case 'V':
                fsh_loglevel = LOGLEVEL_WARNING;
                break;
            case 'S':
                fsh_loglevel = LOGLEVEL_LOG;
                break;
            default:
                abort();
        }
    }

    //Load configs
    fsh_read_config();

    //Start main loop
    fsh_loop();

    //Cleanup
    return EXIT_SUCCESS;
}

void _fsh_exit(int status) {
    exit(status);
}
