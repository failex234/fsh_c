#ifndef FSH_H
#define FSH_H

#define FSH_RL_BUFSIZE 1024
#define FSH_TOK_BUFSIZE 64

#define FSH_VERSION "0.2.1"

#include <stdio.h>

typedef struct config {
    char *path;
    char *ps1;
} Config;

enum LOGLEVEL {
    LOGLEVEL_ERROR,
    LOGLEVEL_WARNING,
    LOGLEVEL_LOG
};

Config *fsh_config;
Var *global_variables;

int last_status;
enum LOGLEVEL fsh_loglevel;

size_t kv_len;


char **var_keys;
char **var_values;
size_t var_len;

//fsh.c
void _fsh_exit(int status);

//utils.c
char *read_line();
char **split_line(char *line);
char **key_and_val_split(char *line, char seperator);
char *parse_ps1();
char *get_home_dir();
char *get_user_name();
char *get_host_name();
char *get_cwd();
int count_chars(const char *str, char c);


//runner.c
int launch(char **args);
int execute(char **args);

//io.c
int fsh_printf(const char *fmt, ...);
int fsh_printerr(const char *fmt, ...);
void fsh_read_config();
char **key_and_val_split_file(FILE *file, char seperator);

//builtins.c
int fsh_num_builtins();
int fsh_cd(char **args);
int fsh_help(char **args);
int fsh_exit(char **args);

//log.c
void fsh_log(enum LOGLEVEL msg_type, const char *msg);
void fsh_logf(enum LOGLEVEL msg_type, const char *fmt, ...);

//vars.c
int is_potential_var_decl(const char *line);
char *substitute_vars(const char *line);
void interpret_var_line(const char *line);
void add_var(const char *key, const char *value);
void remove_var(const char *key);
void change_var(const char *key, const char *newval);

#endif
