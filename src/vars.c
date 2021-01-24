#include "fsh.h"

#include <string.h>
#include <stdlib.h>

int is_potential_var_decl(const char *line) {
    return count_chars(line, '=');
}

char *substitute_vars(const char *line) {
    
}

void interpret_var_line(const char *line) {

}

void add_var(const char *key, const char *value) {
    //TODO: Add Null element to end 
    if (var_len == 0) {
        var_keys = (char**) malloc(1);
        var_values = (char**) malloc(1);
    } else {
        var_keys = (char**) realloc(var_keys, var_len + 1);
        var_values = (char**) realloc(var_values, var_len + 1);
    }

    size_t key_len = strlen(key);
    char *key_ptr = (char*) malloc(key_len + 1);
    size_t val_len = strlen(value);
    char *value_ptr = (char*) malloc(val_len + 1);

    memcpy(key_ptr, key, key_len + 1);
    memcpy(value_ptr, value, val_len + 1);

    var_keys[key_len] = key_ptr;
    var_values[key_len++] = value_ptr;
}

void remove_var(const char *key) {
    if (var_len) {
        //TODO
    }
}

void change_var(const char *key, const char *newval) {

}
