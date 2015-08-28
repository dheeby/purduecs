
/*
 * CS-252
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [ | cmd [arg]* ]* [ [> filename] [< filename] [>& filename] [>> filename] [>>& filename] ]* [&]
 *
 */

%error-verbose

%token	<string_val> WORD SUBSHELL ESCAPEWORD

%token 	NOTOKEN GREAT NEWLINE GREATGREAT GREATGREATAMPERSAND LESS PIPE GREATAMPERSAND AMPERSAND

%union	{
		    char   *string_val;
	    }

%{
//#define yylex yylex
#define MAXFILENAME 1024
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "command.h"
#include <sys/types.h>
#include <sys/wait.h>

void yyerror(const char * s);
int yylex();

%}

%%

goal:	
	commands
	;

commands: 
	command
	| commands command 
	;

command: simple_command
        ;

simple_command:	
	pipe_list iomodifier_list background_optional NEWLINE {
		Command::_currentCommand.execute();
	}
	| NEWLINE {
        Command::_currentCommand.prompt();
    }
	| error NEWLINE { yyerrok; }
	;

pipe_list:
    command_and_args pipe {
        
    }   
    ;

pipe:
    PIPE pipe_list
    | /* can be empty */
    ;

command_and_args:
	command_word arg_list {
		Command::_currentCommand.
			insertSimpleCommand( Command::_currentSimpleCommand );
	}
	;

arg_list:
	arg_list argument
	| /* can be empty */
	;

argument:
	WORD {
            if (strchr($1, '*') == NULL && strchr($1, '?') == NULL) {
                Command::_currentSimpleCommand->insertArgument( $1 );
            } else {
                char str[1];
                str[0] = '\0';
                Command::_currentSimpleCommand->expandWildcard(str, $1);
                free(Command::_currentSimpleCommand->filelist);
            }
	}
    | ESCAPEWORD {
        Command::_currentSimpleCommand->insertArgument( $1 );
    }
    | SUBSHELL {
        int pid;
        
        char * buf = (char *)malloc(sizeof(char) * (strlen($1) + 2));
        char * bufptr = buf;
        
        while (*$1) {
            *bufptr++ = *$1++;
        }
        
        *bufptr++ = '\n';
        *bufptr++ = '\0';
        
        // Save stdin/stdout
        int tmpin = dup(0);
        int tmpout = dup(1);

        int fdpipep[2];
        int fdpipec[2];

        if (pipe(fdpipep) || pipe(fdpipec)) {
            perror("pipe");
            exit(1);
        }
        
        dup2(fdpipep[0], 0);
        dup2(fdpipec[1], 1);
        close(fdpipep[0]);
        close(fdpipec[1]);

        
        if ((pid = fork()) == 0) {
            dup2(fdpipec[0], 0);
            dup2(fdpipep[1], 1);
            close(fdpipec[0]);
            close(fdpipep[1]);
            
            char *args[2];
            char shellcmd[15] = "../shell\0";
            args[0] = shellcmd;
            args[1] = NULL;
            execvp(args[0], args);
            perror("execvp");
            _exit(1);
        } else if (pid< 0) {
            perror("fork");
            exit(1);
        }
        
        write(1, buf, strlen(buf));
        dup2(tmpout, 1);
        close(tmpout);

        close(fdpipep[0]);
        close(fdpipep[1]);
        close(fdpipec[0]);
        close(fdpipep[1]);

        char * outbuf = (char *)malloc(sizeof(char) * 1024);
        char * outbufptr = outbuf;
        
        int totalread = 0;
        int currentMaxLen = 1024;
        while(read(0, outbufptr, 1)) {
            // Reads output from child to outbuf
            if (*outbufptr == '\n') {
                *outbufptr = ' ';
            }
            if (*outbufptr == ' ' && *(outbufptr - 1) == ' ') {
                outbufptr--;
            }
            outbufptr++;
            totalread++;
            if (totalread >= currentMaxLen) { 
                outbuf = (char *)realloc(outbuf, currentMaxLen * 2);
                currentMaxLen *= 2;
            }
        }
        
        *outbufptr = '\0';

        dup2(tmpin, 0);
        close(tmpin);
        
        outbufptr = outbuf;
        char *currentarg = (char *)malloc(strlen(outbuf) + 1);
        char *currentargptr = currentarg;
        while (*outbufptr) {
            if (*outbufptr == ' ') {
                *currentargptr = '\0';
                Command::_currentSimpleCommand->insertArgument( strdup(currentarg) );
                currentargptr = currentarg;
                outbufptr++;
            } else {
                *currentargptr++ = *outbufptr++;
            }
        }
        *currentargptr = '\0'; 
        Command::_currentSimpleCommand->insertArgument( strdup(currentarg) );
        //Command::_currentSimpleCommand->insertArgument( outbuf );
    }
	;

command_word:
	WORD {
	       Command::_currentSimpleCommand = new SimpleCommand();
	       Command::_currentSimpleCommand->insertArgument( $1 );
	}
	;

iomodifier_list:
    iomodifier_list iomodifier
    | /* can be empty */
    ;

iomodifier:
	GREAT WORD {
		if (Command::_currentCommand._outFile) {
            yyerror("Ambiguous output redirect");
        }
        Command::_currentCommand._outFile = $2;
	}
    |
    LESS WORD {
        Command::_currentCommand._inputFile = $2;
    }
    |
    GREATAMPERSAND WORD {
        if (Command::_currentCommand._outFile) {
            yyerror("Ambiguous output redirect");
        }
        Command::_currentCommand._outFile = $2;
        Command::_currentCommand._errFile = strdup($2);
    }
    |
    GREATGREAT WORD {
        if (Command::_currentCommand._outFile) {
            yyerror("Ambiguous output redirect");
        }
        Command::_currentCommand._outFile = $2;
        Command::_currentCommand._appendOutput = 1;
    }
    |
    GREATGREATAMPERSAND WORD {
        if (Command::_currentCommand._outFile) {
            yyerror("Ambiguous output redirect");
        }
        Command::_currentCommand._outFile = $2;
        Command::_currentCommand._errFile = strdup($2);
        Command::_currentCommand._appendOutput = 1;
    }
	;

background_optional:
    AMPERSAND {
        Command::_currentCommand._background = 1;
    }
    | /* can be empty */
    ;

%%

void
yyerror(const char * s)
{
	fprintf(stderr,"%s", s);
}

#if 0
main()
{
	yyparse();
}
#endif
