#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#include "fsh.h"

#define CTRL_KEY(k) ((k) & 0x1f)

struct termios original_termios;

void die(const char *str) {
    perror(str);
    exit(1);
}


void add_to_input_buffer(char c) {
    switch(c) {
        case BACKSPACE:
            if (input_buf_len > 1) {
                input_buf = (char*) realloc(input_buf, --input_buf_len);   
            } else if (input_buf_len == 1) {
                free(input_buf);
                input_buf_len = 0;
            }
            break;
        default:
            if (input_buf_len != 0) {
                input_buf = (char*) realloc(input_buf, ++input_buf_len);
            } else {
                input_buf = (char*) malloc(++input_buf_len); 
            }
            input_buf[input_buf_len - 1] = c;
            break;
    }
}

char *get_input_buf_str() {
    char *input_str = get_input_buf_no_clear();

    if (!last_cmd) {
	last_cmd = (char*) malloc(input_buf_len + 1);
    } else {
	last_cmd = (char*) realloc(last_cmd, input_buf_len + 1);
    }

    memcpy(last_cmd, input_str, input_buf_len + 1);

    free(input_buf);
    input_buf_len = 0;

    return input_str;
}

char *get_input_buf_no_clear() {
    char *input_str = (char*) malloc(input_buf_len + 1);

    memcpy(input_str, input_buf, input_buf_len);
    input_str[input_buf_len] = '\0';

    return input_str;
}

void enable_raw_mode() {
    //Save the current termios to restore it on exit
    if (tcgetattr(STDIN_FILENO, &original_termios) == -1) {
        die("fsh (tcgetattr)");
    }
    //atexit(disable_raw_mode);

    struct termios raw_mode = original_termios;
    raw_mode.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw_mode.c_cflag |= (CS8);
    raw_mode.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    //set the tiemout for read to 100ms
    raw_mode.c_cc[VMIN] = 0;
    raw_mode.c_cc[VTIME] = 1;

    //set the new termios
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_mode) == -1) {
        die("fsh (tcsetattr)");
    }
}

void disable_raw_mode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios) == -1) {
        die("fsh (tcsetattr)");       
    }
}

int getch() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) die("fsh (getch)");
    }

    if (c == '\x1b') {
        char seq[3];

        if (read(STDIN_FILENO, &seq[0], 1) != 1 || read(STDIN_FILENO, &seq[1], 1) != 1) {
			return '\x1b';
		}

        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
				if (read(STDIN_FILENO, &seq[2], 1) != 1) {
					return '\x1b';
				}
				
				if (seq[2] == '~') {
					switch(seq[1]) {
						case '1': return HOME_KEY;
						case '3': return DEL_KEY;
						case '4': return END_KEY;
						case '5': return PAGE_UP;
						case '6': return PAGE_DOWN;
						case '7': return HOME_KEY;
						case '8': return END_KEY;
					}
				}
			} else {
				switch (seq[1]) {
					case 'A': return ARROW_UP;
					case 'B': return ARROW_DOWN;
					case 'C': return ARROW_RIGHT;
					case 'D': return ARROW_LEFT;
					case 'H': return HOME_KEY;
					case 'F': return END_KEY;
				}
			}
        } else if (seq[0] == '0') {
            switch(seq[1]) {
				case 'H': return HOME_KEY;
				case 'F': return END_KEY;
			}
        }
        return '\x1b';
    }
    return c;
}

int process_key_press() {
    int c = getch();
    int nread;

    switch(c) {
        case CTRL_KEY('c'):
	    printf("^C\n");
	    return 1;
        case CTRL_KEY('d'):
            printf("exit\n");
            exitme = 1;
	    return 1;
	case CTRL_KEY('l'):
	    {
	    	char *temp[] = { "clear", NULL };
    	    	launch(temp);
    		printf("%s%s", parse_ps1(), get_input_buf_no_clear());		
	    	break;
	    }
        case ARROW_LEFT:
            break;
        case ARROW_RIGHT:
            break;
        case ARROW_UP:
	    {
		    if (last_cmd) {
			    size_t last_cmd_len = strlen(last_cmd);
			    input_buf = (char*) realloc(input_buf, last_cmd_len + 1);
			    memcpy(input_buf, last_cmd, last_cmd_len + 1);

			    if (last_cmd_len) {
				    free(last_cmd);
			    }
			    last_cmd = NULL;

			    while(input_buf_len--) {
				    putc(8, stdout);
			    }

			    printf("%s", input_buf);

			    input_buf_len = last_cmd_len;
			    cursor_pos_x = ps1_len + input_buf_len;
		    }
            	break;
	    }
        case ARROW_DOWN:
	    //TODO
            break;
        case HOME_KEY:
            break;
        case END_KEY:
            break;
        case PAGE_UP:
            break;
        case PAGE_DOWN:
        case '\r':
        case '\n':
            printf("\n");
	    cursor_pos_x = ps1_len;
            return 1;
        case '\t':
            break;
        case CTRL_KEY('h'):
        case DEL_KEY:
        case BACKSPACE:
	    if (cursor_pos_x > ps1_len) {
		printf("%c", 8);
		printf("%c", ' ');
		printf("%c", 8);
		add_to_input_buffer(BACKSPACE);

		cursor_pos_x--;
	    }
            break;
	default:
	    printf("%c", c);
	    add_to_input_buffer(c);
	    cursor_pos_x++;
    }
    fflush(stdout);
    return 0;
}
