#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <limits.h>

#include "fsh.h"

char *read_line() {
    int pos = 0, c, curr_buf_size = FSH_RL_BUFSIZE;
    char *buf = (char*) malloc(curr_buf_size); 

    if (!buf) {
        fsh_printerr("memory allocation error\n");
        _fsh_exit(EXIT_FAILURE);
    }

    while(1) {
        c = getchar();


        if (c == EOF || c == '\n') {
            buf[pos] = '\0';
            return buf;
        } else {
            buf[pos++] = c;
        }

        if (pos >= curr_buf_size) {
            curr_buf_size += FSH_RL_BUFSIZE;
            buf = (char*) realloc(buf, curr_buf_size);

            if (!buf) {
                fsh_printerr("error reallocting buffer memory\n");
                _fsh_exit(EXIT_FAILURE);
            }
        }
    }
}

char **split_line(char *line) {
    int bufsize = FSH_TOK_BUFSIZE, tokens_pos = 0, token_pos = 0, in_quotes = 0, inserted_chars = 0;
    int bufsize_token = FSH_TOK_BUFSIZE;
    char **tokens = (char**) malloc(bufsize);
    char *token = (char*) malloc(bufsize);
    char *c = (char*) line;

    if (!tokens) {
        fsh_printerr("error allocating memory for tokens\n");
        _fsh_exit(EXIT_FAILURE);
    }

    while(*c) {
        if (inserted_chars >= bufsize) {
            bufsize += FSH_TOK_BUFSIZE;
            tokens = (char**) realloc(tokens, bufsize);

            if (!tokens) {
                fsh_printerr("error allocating more space for tokens\n");
                _fsh_exit(EXIT_FAILURE);
            }
        }

        if (token_pos >= bufsize_token) {
            bufsize_token += FSH_TOK_BUFSIZE;
            token = (char*) realloc(token, bufsize_token);

            if (!token) {
                fsh_printerr("error reallocating more space for token\n");
                _fsh_exit(EXIT_FAILURE);
            }
        }

        switch(*c) {
            case '"':
                in_quotes = !in_quotes;
                break;
            case ' ':
            case '\t':
            case '\n':
            case '\a':
                if (!in_quotes) {
                    token[token_pos] = '\0';
                    tokens[tokens_pos++] = token;
                    inserted_chars--;
                    token_pos = 0;

                    token = (char*) malloc(bufsize);
                } else {
                    token[token_pos++] = *c;
                }
                break;
            case '\\':
            default:
                token[token_pos++] = *c;
                break;
        }
        c++;
        inserted_chars++;
    }

    if (in_quotes) {
        fsh_printf("missing closing quote\n");
    }

    token[token_pos] = '\0';
    tokens[tokens_pos++] = token;
    tokens[tokens_pos] = '\0';
    
    return tokens;
}


char **key_and_val_split(char *line, char seperator) {
    int bufsize = FSH_TOK_BUFSIZE, tokens_pos = 0, token_pos = 0, in_quotes = 0, inserted_chars = 0;
    int bufsize_token = FSH_TOK_BUFSIZE;
    char **tokens = (char**) malloc(bufsize);
    char *token = (char*) malloc(bufsize);
    char *c = (char*) line;

    if (!tokens) {
        fsh_printerr("error allocating memory for tokens\n");
        _fsh_exit(EXIT_FAILURE);
    }

    while(*c) {
        if (inserted_chars >= bufsize) {
            bufsize += FSH_TOK_BUFSIZE;
            tokens = (char**) realloc(tokens, bufsize);

            if (!tokens) {
                fsh_printerr("error allocating more space for tokens\n");
                _fsh_exit(EXIT_FAILURE);
            }
        }

        if (token_pos >= bufsize_token) {
            bufsize_token += FSH_TOK_BUFSIZE;
            token = (char*) realloc(token, bufsize_token);

            if (!token) {
                fsh_printerr("error reallocating more space for token\n");
                _fsh_exit(EXIT_FAILURE);
            }
        }

        if (*c == '"') {
            in_quotes= !in_quotes;
        } else if (*c == ' ' || *c == seperator) {
            if (!in_quotes) {
                token[token_pos] = '\0';
                tokens[tokens_pos++] = token;
                inserted_chars--;
                token_pos = 0;

                token = (char*) malloc(bufsize);
            } else {
                token[token_pos++] = *c;
            }
        } else {
            token[token_pos++] = *c;
        }
        c++;
        inserted_chars++;
    }

    if (in_quotes) {
        fsh_printf("missing closing quote\n");
    }

    token[token_pos] = '\0';
    tokens[tokens_pos++] = token;
    tokens[tokens_pos] = '\0';
    
    return tokens;
}

char *get_home_dir() {
    char *home;

    home = getenv("HOME");

    if (!home) {
        fsh_log(LOGLEVEL_WARNING, "cannot get HOME from environment, trying getpwuid\n");
        uid_t user = getuid();

        home = getpwuid(user)->pw_dir;

        if (!home) {
            fsh_log(LOGLEVEL_ERROR, "unable to get home directory!!\n");
            return NULL;
        }
    }

    return home;
}

char *parse_ps1() {

    char *user_name, *host_name, *cwd;
    char *ps1_ptr = fsh_config->ps1;
    char *new_ps1 = (char*) malloc(128);

    int ps1_idx = 0;

    while(*ps1_ptr) {
        if (*ps1_ptr == '\\') {
            ps1_ptr++;
            if (*ps1_ptr == 'u') {
                user_name = get_user_name();
                size_t lenuser = strlen(user_name);
                memcpy(&new_ps1[ps1_idx], user_name, lenuser);

                ps1_idx += lenuser;
            } else if (*ps1_ptr == 'h') {
                host_name = get_host_name();
                size_t lenhost = strlen(host_name);
                memcpy(&new_ps1[ps1_idx], host_name, lenhost);

                ps1_idx += lenhost;
            } else if (*ps1_ptr == 'w') {
                cwd = get_cwd();
                size_t lenwd = strlen(cwd);
                memcpy(&new_ps1[ps1_idx], cwd, lenwd);

                ps1_idx += lenwd;
            } else if (*ps1_ptr == '$') {
                uid_t user = getuid();
                char uidchar = (user == 0 ? '$' : '#');
                new_ps1[ps1_idx++] = uidchar;
            } else {
                new_ps1[ps1_idx++] = *ps1_ptr;
            }
        } else {
            new_ps1[ps1_idx++] = *ps1_ptr;
        }
        ps1_ptr++;
    }

    new_ps1[ps1_idx] = '\0';

    free(host_name);
    return new_ps1;
}


char *get_user_name() {
    char *ret;
    ret = getenv("USER");

    if (!ret) {
        char *buf = (char*) malloc(128);
        getlogin_r(buf, 128);

        ret = (char*) malloc(strlen(buf) + 1);
        memcpy(ret, buf, strlen(buf) + 1);
        free(buf);

    }

    return ret;
}

char *get_host_name() {
    //TODO: Shorten hostname to first period
    char *ret;
    char *host = (char*) malloc(HOST_NAME_MAX + 1);
    gethostname(host, HOST_NAME_MAX + 1);

    ret = (char*) malloc(strlen(host) + 1);
    memcpy(ret, host, strlen(host) + 1);

    free(host);

    return ret;
}

char *get_cwd() {
    char *ret;
    char *cwd = (char*) malloc(PATH_MAX + 1);
    getcwd(cwd, PATH_MAX + 1);

    char *home = get_home_dir();

    //TODO: Replace home directory with tilde, in all possible paths!!!
    //ex /home/felix/fsh _> ~/fsh
    if (!strcmp(cwd, home)) {
        ret = (char*) malloc(2);
        ret[0] = '~';
        ret[1] = '\0'; 
    } else {
        ret = (char*) malloc(strlen(cwd));
        memcpy(ret, cwd, strlen(cwd) + 1);
    } 
    free(cwd);

    return ret;
}
