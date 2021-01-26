#ifndef FSH_H
#define FSH_H

#define FSH_RL_BUFSIZE 1024
#define FSH_TOK_BUFSIZE 64
#define HOST_NAME_MAX 32

#define FSH_VERSION "0.4.2"

#include <stdio.h>
#include <stdint.h>


typedef struct config {
    char *ps1;
    char *path;
    uint8_t fg_color;
    uint8_t bg_color;
} Config;

enum LOGLEVEL {
    LOGLEVEL_ERROR,
    LOGLEVEL_WARNING,
    LOGLEVEL_LOG
};

enum keys {
    BACKSPACE = 127,
    ARROW_LEFT 	= 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN,
};

enum colors {
    BLACK = 30,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    RESET = 0
};

Config *fsh_config;
char *ps1;
size_t ps1_len;

int last_status;
int exitme;
enum LOGLEVEL fsh_loglevel;

size_t kv_len;


char **var_keys;
char **var_values;
size_t var_len;

char *last_cmd;
char *input_buf;
size_t input_buf_len;

int cursor_pos_x;

FILE *fsh_log_file;

//fsh.c
void _fsh_exit(int status);

//configs.c
void openlog();
void make_config_folder();
FILE *get_fsh_file();
char *get_fsh_path();
void fsh_read_config();

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
int search_in_path(const char *binarypath);


//runner.c
int launch(char **args);
int execute(char **args);

//io.c
int fsh_printf(const char *fmt, ...);
int fsh_printerr(const char *fmt, ...);
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

//terminal.c
void enable_raw_mode();
void disable_raw_mode();
void set_fg_color(uint8_t color);
void set_bg_color(uint8_t color);
char *get_color();
char *get_single_color(int bg);
void reset_color();
int process_key_press();
char *get_input_buf_str();
char *get_input_buf_no_clear();
char *style_text(const char *text);

#endif
