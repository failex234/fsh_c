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
        "cd [directory]\r\n"
        "\r\n"
        "Where directory can be a relative or absolute path.\r\n"
        "You can also leave out [directory] to change the working\r\n"
        "directory to your home directory.\r\n"
        "\r\n"
        "examples:\r\n"
        "\tcd foo/bar\r\n"
        "\tcd /usr\r\n"
        "\tcd",

        "help [command]\r\n"
        "\r\n"
        "Where command can be any builtin fsh command.\r\n"
        "specify a command to get more information about the\r\n"
        "particular command or type help without any arguments\r\n"
        "to list all available builtin fsh commands\r\n"
        "\r\n"
        "examples:\r\n"
        "\thelp cd\r\n"
        "\thelp",

        "exit\r\n"
        "\r\n"
        "Close the current fsh session and return to previous program.\r\n"
        "no arguments are needed for exit\r\n"
        "\r\n"
        "examples:\r\n"
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
        if (chdir(get_home_dir()) != 0) {
            perror("fsh");
        }
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
        printf("Felix Naumann's FSH\r\n");
        printf("Type program names and arguments, and hit enter.\r\n");
        printf("The following are built in:\r\n");

        for (i = 0; i < fsh_num_builtins(); i++) {
            printf(" %s - %s\r\n", builtin_str[i], builtin_desc[i]);
        }

        printf("Use the man command for information on other programs.\r\n");
    } else {
        int builtin_idx = -1;
        for (int i = 0; i < fsh_num_builtins(); i++) {
            if (!strcmp(args[1], builtin_str[i])) {
                builtin_idx = i;
            }
        }
        if (builtin_idx != -1) {
            printf("usage: %s\r\n", builtin_help[builtin_idx]);
        } else {
            fsh_printerr("fsh: comamnd not found!\r\n");
        }
    }

    return 1;
}

int fsh_exit(char **args) {
    exitme = 1;
    return 0;
}
