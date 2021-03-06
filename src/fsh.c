#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>

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
    printf("%s (%s %s)\n", FSH_VERSION, __DATE__, __TIME__);
}

void fsh_loop() {
    char *line, **args;
    size_t line_len;
    int status = 0;

    last_cmd = NULL;
    input_buf = NULL;
    input_buf_len = 0;

    do {
        ps1 = parse_ps1();
        ps1_len = strlen(ps1);
        cursor_pos_x = (int) ps1_len;
        fsh_printf("%s", ps1);
        fflush(stdout);

        //Get input and split it correctly so that we can
        //parse and interpret it
        while(!process_key_press());
        if (exitme) break;

        line = get_input_buf_str();
        line_len = strlen(line);

        if (line_len) {
            args = split_line(line);

            fsh_logf(LOGLEVEL_LOG, "Running command %s", args[0]);
            status = execute(args);
            reset_color();

            size_t line_len = strlen(line);
            last_cmd = (char*) realloc(last_cmd, line_len + 1);
            memcpy(last_cmd, line, line_len + 1);

            free(line);

            char **args_ptr = args;

            while(*args) {
                free(*args);
                args++;
            }
            free(args_ptr);
        }

    } while (!exitme);
}


int main(int argc, char **argv) {
    fsh_loglevel = LOGLEVEL_ERROR;
    exitme = 0;

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

    make_config_folder();
    openlog();
    //Load configs
    fsh_read_config();

    enable_raw_mode();

    reset_color();

    //Start main loop
    fsh_loop();

    disable_raw_mode();
    reset_color();

    //Cleanup
    return EXIT_SUCCESS;
}

void _fsh_exit(int status) {
    exit(status);
}
