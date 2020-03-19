#include <stdlib.h>

#include "fsh.h"

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
    //Arguments
    fsh_loglevel = LOGLEVEL_ERROR;
    //Load configs
    fsh_read_config();
    fsh_loop();

    //Cleanup
    return EXIT_SUCCESS;
}

void _fsh_exit(int status) {
    exit(status);
}
