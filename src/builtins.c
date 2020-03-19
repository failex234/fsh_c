#include <unistd.h>
#include <string.h>
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

char *builtin_help[] = {
    "cd [directory]\n"
    "\n"
    "Where directory can be a relative or absolute path.\n"
    "You can also leave out [directory] to change the working\n"
    "directory to your home directory.\n"
    "\n"
    "examples:\n"
    "\tcd foo/bar\n"
    "\tcd /usr\n"
    "\tcd",

    "help [command]\n"
    "\n"
    "Where command can be any builtin fsh command.\n"
    "specify a command to get more information about the\n"
    "particular command or type help without any arguments\n"
    "to list all available builtin fsh commands\n"
    "\n"
    "examples:\n"
    "\thelp cd\n"
    "\thelp",

    "exit\n"
    "\n"
    "Close the current fsh session and return to previous program.\n"
    "no arguments are needed for exit\n"
    "\n"
    "examples:\n"
    "\texit"
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
        int builtin_idx = -1;
        for (int i = 0; i < fsh_num_builtins(); i++) {
            if (!strcmp(args[1], builtin_str[i])) {
                builtin_idx = i;
            }
        }
        if (builtin_idx != -1) {
            printf("usage: %s\n", builtin_help[builtin_idx]);
        } else {
            fsh_printerr("fsh: comamnd not found!\n");
        }
    }

    return 1;
}

int fsh_exit(char **args) {
    return 0;
}
