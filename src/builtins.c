#include <unistd.h>
#include "fsh.h"

char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

char *builtin_desc[] = {
    "change your working directory",
    "show fsh help (this menu)",
    "exit fsh"
};

int (*builtin_func[]) (char**) = {
    &fsh_cd,
    &fsh_help,
    &fsh_exit
};

int fsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int fsh_cd(char **args) {
    if (args[1] == NULL) {
        fsh_printf("go to home directory NOT IMPLEMENTED!\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("fsh");
        }
    }

    return 1;
}

int fsh_help(char **args) {
    if (args[1] == NULL) {
        int i;
        printf("Felix Naumann's FSH\n");
        printf("Type program names and arguments, and hit enter.\n");
        printf("The following are built in:\n");

        for (i = 0; i < fsh_num_builtins(); i++) {
            printf(" %s - %s\n", builtin_str[i], builtin_desc[i]);
        }

        printf("Use the man command for information on other programs.\n");
    } else {
        printf("Implement me, to show help for wanted command!\n");
    }

    return 1;
}

int fsh_exit(char **args) {
    return 0;
}
