/*
 * CS354: Operating Systems. 
 * Purdue University
 * Example that shows how to read one line with simple editing
 * using raw terminal.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <regex.h>
#include <dirent.h>

#define MAX_BUFFER_LINE 2048

// Buffer where line is stored
int line_length;
int line_pos;
char line_buffer[MAX_BUFFER_LINE];

int history_index = 0;
int history_count = 0;
char * history[50];

char **tablist;
int tablistindex = 0;
int tablistsize = 0;
int tablistset = 0;
int spaceindex = 0;

void read_line_print_usage()
{
    char * usage = "\n"
        " Usage(ctrl-?)        Print usage\n"
        " Backspace(ctrl-H)    Deletes the previous character\n"
        " Delete(ctrl-D)       Deletes the next character\n"
        " Up arrow             See the next oldest command in the history\n"
        " Down arrow           See the next newest command in the history\n"
        " Left arrow           Move the cursor to the left\n"
        " Right arrow          Move the cursor to the right\n"
        " End(ctrl-E)          Move cursor to the end of the line\n"
        " Home(ctrl-A)         Move cursor to the beginning of the line\n"
        " Tab                  Auto complete matching entries\n";

    write(1, usage, strlen(usage));
}

/* 
 * Input a line with some basic editing.
 */
char * read_line() {
    struct termios orig_attr;
    tcgetattr(0, &orig_attr);
    // Set terminal in raw mode
    tty_raw_mode();

    line_length = 0;
    line_pos = 0;

    // Read one line until enter is typed
    while (1) {

        // Read one character in raw mode.
        char ch;
        read(0, &ch, 1);

        if (ch >= 32 && ch != 127) {
            // It is a printable character. 
            
            int i;
            for (i = line_length; i > line_pos; i--) {
                line_buffer[i] = line_buffer[i - 1];
            }
            line_buffer[i] = ch;
            line_length++;

            if (line_length == MAX_BUFFER_LINE - 2) break;

            for (i = line_pos; i < line_length; i++) {
                ch = line_buffer[i];
                write(1, &ch, 1);
            }
            
            ch = 8;
            for (i = line_length; i > line_pos + 1; i--) {
                write(1, &ch, 1);
            }

            line_pos++;
        }
        else if (ch == 10) {
            // <Enter> was typed. Return line
            // Print newline
            write(1,&ch,1);

            break;
        }
        else if (ch == 31) {
            // ctrl-?
            read_line_print_usage();
            line_buffer[0]=0;
            break;
        }
        else if (ch == 8 || ch == 127) {
            // <backspace> was typed. Remove previous character read.
            if (line_length == 0 || line_pos == 0) {
                continue;
            } else { 
                int i;
                
                for (i = line_pos - 1; i < line_length - 1; i++) {
                    line_buffer[i] = line_buffer[i + 1];
                }
                ch = 8;
                write(1, &ch, 1);

                for (i = line_pos; i < line_length; i++) {
                    ch = line_buffer[i - 1];
                    write(1, &ch, 1);
                }

                ch = ' ';
                write(1, &ch, 1);
                ch = 8;
                write(1, &ch, 1);
                
                for (i = line_pos; i < line_length; i++) {
                    write(1, &ch, 1);
                }

                line_pos--;
                line_length--;
            }
        }
        else if (ch == 27) {
            // Escape sequence. Read two chars more
            //
            // HINT: Use the program "keyboard-example" to
            // see the ascii code for the different chars typed.
            //
            char ch1; 
            char ch2;
            read(0, &ch1, 1);
            read(0, &ch2, 1);
            if (ch1 == 91 && ch2 == 65) {
	            // Up arrow. Print next line in history.
                if (history_count == 0 || history_index == 0) {
                    continue;
                }
	            // Erase old line
	            // Print backspaces
	            int i = 0;
	            for (i =0; i < line_length; i++) {
	                ch = 8;
	                write(1, &ch, 1);
	            }

	            // Print spaces on top
	            for (i =0; i < line_length; i++) {
	                ch = ' ';
	                write(1, &ch, 1);
	            }

	            // Print backspaces
	            for (i =0; i < line_length; i++) {
	                ch = 8;
	                write(1, &ch, 1);
	            }

	            // Copy line from history
	            if (history_index > 0) {
                    history_index--;
                }
                strcpy(line_buffer, history[history_index]);
                line_length = strlen(line_buffer);

	            // echo line
	            write(1, line_buffer, line_length);
                line_pos = line_length;
            } else if (ch1 == 91 && ch2 == 66) {
                // Down arrow. Print previous line in history
                if (history_count == 0 || history_index == history_count) {
                    line_pos = 0;
                    line_length = 0;
                    continue;
                }

                int i = 0;
                for (i = 0; i < line_length; i++) {
                    ch = 8;
                    write(1, &ch, 1);
                }

                for (i = 0; i < line_length; i++) {
                    ch = ' ';
                    write(1, &ch, 1);
                }

                for (i = 0; i < line_length; i++) {
                    ch = 8;
                    write(1, &ch, 1);
                }
                
                history_index++;
                if (history_index == history_count) {
                    line_pos = 0;
                    line_length = 0;
                    continue;
                }
                
                strcpy(line_buffer,history[history_index]);
                line_length = strlen(line_buffer);

                write(1, line_buffer, line_length);
                line_pos = line_length;
            } else if (ch1 == 91 && ch2 == 67) {
                // Right arrow. Move line position to the right
                if (line_pos < line_length) {
                    ch = line_buffer[line_pos++];
                    write(1, &ch, 1);
                }
            } else if (ch1 == 91 && ch2 == 68) {
                // Left arrow. Move line position to the left
                if (line_pos > 0) {
                    ch = 8;
                    write(1, &ch, 1);
                    line_pos--;
                }
            } else if (ch1 == 79 && ch2 == 72) {
                ch = 8;
                while (line_pos > 0) {
                    write(1, &ch, 1);
                    line_pos--;
                }
            } else if (ch1 == 79 && ch2 == 70) {
                while (line_pos < line_length + 1) {
                    ch = line_buffer[line_pos - 1];
                    write(1, &ch, 1);
                    line_pos++;
                }
                line_pos--;
            }
      
        } else if (ch == 1) {
            // Home button. Move line position to start
            ch = 8;
            while (line_pos > 0) {
                write(1, &ch, 1);
                line_pos--;
            }
        } else if (ch == 5) {
            // End button. Move line position to end
            while (line_pos < line_length + 1) {
                ch = line_buffer[line_pos - 1];
                write(1, &ch, 1);
                line_pos++;
            }
            line_pos--;
        } else if (ch == 4) {
            // Delete. Move characters following current position to the left one
            if (line_length == 0 || line_pos == line_length) {
                continue;
            } else { 
                int i;
                
                for (i = line_pos; i < line_length - 1; i++) {
                    line_buffer[i] = line_buffer[i + 1];
                }

                line_length--;

                for (i = line_pos; i < line_length; i++) {
                    ch = line_buffer[i];
                    write(1, &ch, 1);
                }

                ch = ' ';
                write(1, &ch, 1);
                ch = 8;
                write(1, &ch, 1);

                for (i = line_pos; i < line_length; i++) {
                    write(1, &ch, 1);
                }
            }
        } else if (ch == 9) {
            // Tab. Path completion
            // TODO: extra credit
            if (tablistset == 0) {
                char * regex = (char *)malloc(sizeof(char) * (line_length + 5));
                regex[0] = '^';

                int i;
                for (i = line_length - 1; i >= 0; i--) {
                    if (line_buffer[i] == ' ') {
                        spaceindex = i;
                        break;
                    }
                }

                if (spaceindex != 0) {
                    int copysize = (line_buffer + line_length) - (line_buffer + spaceindex) - 1;
                    strncpy(regex + 1, line_buffer + spaceindex + 1, copysize);
                    regex[copysize + 1] = '.';
                    regex[copysize + 2] = '*';
                    regex[copysize + 3] = '$';
                    regex[copysize + 4] = '\0';
                } else {
                    strncpy(regex + 1, line_buffer, line_length);
                    regex[line_length] = '.';
                    regex[line_length + 1] = '*';
                    regex[line_length + 2] = '$';
                    regex[line_length + 3] = '\0';
                }

                regex_t re;
                int regexbuf = regcomp(&re, regex, REG_EXTENDED | REG_NOSUB);
                if (regexbuf != 0) {
                    perror("regcomp");
                    return;
                }

                char * d = (char *)malloc(sizeof(char) * 2);
                d[0] = '.';
                d[1] = '\0';

                DIR * dir = opendir(d);
                if (dir == NULL) {
                    perror("opendir");
                    return;
                }

                free(d);
                
                struct dirent * ent;
                regmatch_t match;
                int maxEntries = 10;
                tablistsize = 0;
                tablist = (char **)malloc(maxEntries * sizeof(char *));
                while ((ent = readdir(dir)) != NULL) {
                    if (regexec(&re, ent->d_name, 1, &match, 0) == 0) {
                        if (tablistsize == maxEntries) {
                            maxEntries *= 2;
                            tablist = (char **)realloc(tablist, maxEntries * sizeof(char *));
                        }
                        tablist[tablistsize++] = strdup(ent->d_name);
                    }
                }
                tablistset = 1;
            }

            int i;
            ch = 8;
            for (i = 0; i < line_length; i++) {
                write(1, &ch, 1);
            }
            ch = ' ';
            for (i = 0; i < line_length; i++) {
                write(1, &ch, 1);
            }
            ch = 8;
            for (i = 0; i < line_length; i++) {
                write(1, &ch, 1);
            }

            if (spaceindex == 0) {
                strcpy(line_buffer, tablist[tablistindex]);
                line_length = strlen(line_buffer);    
            } else {
                char * tmptab = tablist[tablistindex];
                for (i = 0; i < strlen(tmptab); i++) {
                    line_buffer[spaceindex + i + 1] = tmptab[i];
                }
                line_length = spaceindex + strlen(tablist[tablistindex]) + 1;
            }

            write(1, line_buffer, line_length);
            line_pos = line_length;
            if (tablistindex < tablistsize - 1) {
                tablistindex++;
            }
        }
    }
    
    if (strlen(line_buffer) > 1) {
        char * tmpbuf = (char *)malloc(sizeof(char) * (line_length + 2));
        strncpy(tmpbuf, line_buffer, line_length);
        tmpbuf[line_length] = 0;
        history[history_count++] = strdup(tmpbuf);
        free(tmpbuf);
    }

    history_index = history_count;

    // Add eol and null char at the end of string
    line_buffer[line_length] = 10;
    line_length++;
    line_buffer[line_length] = 0;
    if (tablistset) {
        spaceindex = 0;
        tablistset = 0;
        tablistindex = 0;
        free(tablist);
    }

    tcsetattr(0, TCSANOW, &orig_attr);
    return line_buffer;
}

