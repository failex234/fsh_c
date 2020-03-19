#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "fsh.h"

extern char *builtin_str[];
extern char *builtin_desc[];
extern int (*builtin_func[]) (char**);

int launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();

    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("fsh");
        }
        _fsh_exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fsh");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int execute(char **args) {
    int i;

    if (args[0] == NULL) {
        return 1;
    }

    for (i = 0; i < fsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return launch(args);
}
