
/*
 * CS252: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#include <fcntl.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <pwd.h>
#include <regex.h>
#include <dirent.h>

#include "command.h"

/* ctrl-c handler */
extern "C" void ctrlc_handler(int sig) {
    Command::_currentCommand.clear();
    fprintf(stdout, "\n");
    Command::_currentCommand.prompt();
}

/* zombie process handler */
void zombie_handler(int sig) {
    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}   
}

SimpleCommand::SimpleCommand()
{
	// Creat available space for 5 arguments
	_numberOfAvailableArguments = 5;
	_numberOfArguments = 0;
	_arguments = (char **) malloc( _numberOfAvailableArguments * sizeof( char * ) );
}

void
SimpleCommand::insertArgument( char * argument )
{
    if (strchr(argument, '\\') != NULL) {
        escapeCharacters(argument);
    }
    if (strchr(argument, '$') != NULL) {
        expandEnvar(argument);
    }

    if (*argument == '~') {
        expandTilde(argument);
    }
	if ( _numberOfAvailableArguments == _numberOfArguments + 1 ) {
		// Double the available space
		_numberOfAvailableArguments *= 2;
		_arguments = (char **) realloc( _arguments,
				  _numberOfAvailableArguments * sizeof( char * ) );
	}
	
	_arguments[ _numberOfArguments ] = argument;

	// Add NULL argument at the end
	_arguments[ _numberOfArguments + 1] = NULL;
	
	_numberOfArguments++;
}

void
SimpleCommand::escapeCharacters(char * arg)
{
    char * result = (char *)malloc(sizeof(char) * strlen(arg));
    char * resptr = result;
    char * argptr = arg;

    while (*argptr) {
        if (*(argptr + 1) == '\0') {
            argptr++;
        }
        else if (*argptr == '\\' && *(argptr + 1)) {
            *resptr++ = *(argptr + 1);
            argptr += 2;
        } else {
            *resptr++ = *argptr++;
        }
    }
    *resptr == '\0';
    strcpy(arg, result);
}

void
SimpleCommand::expandEnvar(char * arg)
{
    const char * regex = "[${][^ \t\n][^ \t\n].*[}]\0";
    
    regex_t re;

    int regexbuf = regcomp(&re, regex, REG_EXTENDED | REG_NOSUB);
    if (regexbuf != 0) {
        perror("regcomp");
        return;
    }
    
    regmatch_t match;

    char * argptr = arg;
    while (regexec(&re, argptr, 1, &match, 0) == 0) {
        // Points to start of envar name
        char * varptrs = strchr(argptr, '{') + 1;
        // Used to hold envar name
        char * varbuf = (char *)malloc(sizeof(char) * 1024);
        // Pointer to envar name
        char * varbufptr = varbuf;
        while (*varptrs != '}') {
            *varbufptr++ = *varptrs++;
        }
        *varbufptr == '\0';
        // Get value of envar
        char * varval = getenv(varbuf);

        char * tmparg = (char *)malloc(sizeof(char) * 1024);
        char * tmpargptr = tmparg;
        while (*argptr != '$') {
            *tmpargptr++ = *argptr++;
        }
        argptr += 2;
        
        while (*varval) {
            *tmpargptr++ = *varval++;
        }

        while (*argptr != '}') {
            argptr++;
        }
        argptr++;
        while (*argptr) {
            *tmpargptr++ = *argptr++;
        }
        strcpy(arg, tmparg);
        argptr = arg;
    }
}

void
SimpleCommand::expandWildcard(char * prefix, char * suffix)
{
    if (suffix[0] == 0) {
        return;
    }
    char * s = strchr(suffix, '/');
    char component[1024];
    if (s != NULL) {
        if ((s - suffix) < 1) {
            component[0] = '\0';
        } else {
            strncpy(component, suffix, s - suffix);
        }
        suffix = s + 1;
    } else {
        strcpy(component, suffix);
        suffix = suffix + strlen(suffix);
    }

    char newPrefix[1024];
    if ((strchr(component, '*') == NULL) && (strchr(component, '?') == NULL)) {
        if (strlen(prefix) == 1 && *prefix == '/') {
            sprintf(newPrefix, "/%s", component);
        } else {
            sprintf(newPrefix, "%s/%s", prefix, component);
        }
        expandWildcard(strdup(newPrefix), strdup(suffix));
        return;
    }
    char * regex = (char *)malloc(strlen(component) * 2 + 3);
    char * argptr = component;
    char * regexptr = regex;
    *regexptr++ = '^';
    while (*argptr) {
        if (*argptr == '*') {
            *regexptr++ = '.';
            *regexptr++ = '*';
        } else if (*argptr == '?') {
            *regexptr++ = '.';
        } else if (*argptr == '.') {
            *regexptr++ = '\\';
            *regexptr++ = '.';
        }   else {
            *regexptr++ = *argptr;
        }
        *argptr++;
    }
    *regexptr++ = '$';
    *regexptr = '\0';
    
    regex_t re;

    int regexbuf = regcomp(&re, regex, REG_EXTENDED | REG_NOSUB);
    if (regexbuf != 0) {
        perror("regcomp");
        return;
    }

    char * d;

    if (prefix[0] == 0) {
        d = (char *)malloc(sizeof(char) * 2);
        d[0] = '.';
        d[1] = '\0';
    } else {
        d = strdup(prefix);
    }

    DIR * dir = opendir(d);
    if (dir == NULL) {
        return;
    }

    struct dirent * ent;
    regmatch_t match;
    int maxEntries = 10;
    int nEntries = 0;
    filelist = (char **)malloc(maxEntries * sizeof(char *));
    while ((ent = readdir(dir)) != NULL) {
        if (regexec(&re, ent->d_name, 1, &match, 0) == 0) {
            if (strlen(prefix) == 0) {
                sprintf(newPrefix, "%s", strdup(ent->d_name));
            } else if (strlen(prefix) == 1 && *prefix == '/') {
                sprintf(newPrefix, "/%s", strdup(ent->d_name));
            } 
            else {
                sprintf(newPrefix, "%s/%s", prefix, strdup(ent->d_name));
            }
            expandWildcard(strdup(newPrefix), strdup(suffix));
            
            if (nEntries == maxEntries) {
                maxEntries *= 2;
                filelist = (char **)realloc(filelist, maxEntries * sizeof(char *));
            }
            if (*ent->d_name == '.') {
                if (*component == '.') {
                    filelist[nEntries++] = strdup(ent->d_name);
                }
            } else {
                if (strlen(suffix) ==0) {
                    filelist[nEntries++] = strdup(newPrefix);
                }
            }
        }
    }
    
    sortArray(filelist, nEntries);
        
    for (int i = 0; i < nEntries; i++) {
        Command::_currentSimpleCommand->insertArgument(strdup(filelist[i]));
    }
    
    closedir(dir);
}

void
SimpleCommand::sortArray(char **&arr, int n)
{
    int swapped = 0;
    while (!swapped) {
        swapped = 1;
        for (int i = 0; i < n - 1; i++) {
            if (strcmp(arr[i], arr[i + 1]) > 0) {
                char * tmp = arr[i + 1];
                arr[i + 1] = arr[i];
                arr[i] = tmp;
                swapped = 0;
            }
        }
    }
}

void
SimpleCommand::expandTilde(char * arg)
{
    if ((strcmp(arg, "~") == 0) || (strcmp(arg, "~/") == 0)) {
        strcpy(arg, getpwnam(getenv("USER"))->pw_dir);
    } else {
        char * path = (char *)malloc((6 + strlen(arg)) * sizeof(char));
        const char * prefix = "/homes/";
        char * pathptr = path;
        char * argptr = arg;

        while (*prefix) {
            *path++ = *prefix++;
        }
    
        argptr++;
        while (*argptr) {
            *path++ = *argptr++;
        }
        *path = '\0';
        strcpy(arg, pathptr);
    }
}


Command::Command()
{
	// Create available space for one simple command
	_numberOfAvailableSimpleCommands = 1;
	_simpleCommands = (SimpleCommand **)
		malloc( _numberOfSimpleCommands * sizeof( SimpleCommand * ) );

	_numberOfSimpleCommands = 0;
	_outFile = 0;
	_inputFile = 0;
	_errFile = 0;
	_background = 0;
    _appendOutput = 0;
}

void
Command::insertSimpleCommand( SimpleCommand * simpleCommand )
{
	if ( _numberOfAvailableSimpleCommands == _numberOfSimpleCommands ) {
		_numberOfAvailableSimpleCommands *= 2;
		_simpleCommands = (SimpleCommand **) realloc( _simpleCommands,
			 _numberOfAvailableSimpleCommands * sizeof( SimpleCommand * ) );
	}
	
	_simpleCommands[ _numberOfSimpleCommands ] = simpleCommand;
	_numberOfSimpleCommands++;
}

void
Command:: clear()
{
	for ( int i = 0; i < _numberOfSimpleCommands; i++ ) {
		for ( int j = 0; j < _simpleCommands[ i ]->_numberOfArguments; j ++ ) {
			free ( _simpleCommands[ i ]->_arguments[ j ] );
		}
		
		free ( _simpleCommands[ i ]->_arguments );
		free ( _simpleCommands[ i ] );
	}

	if ( _outFile ) {
		free( _outFile );
	}

	if ( _inputFile ) {
		free( _inputFile );
	}

	if ( _errFile ) {
		free( _errFile );
	}

	_numberOfSimpleCommands = 0;
	_outFile = 0;
	_inputFile = 0;
	_errFile = 0;
	_background = 0;
    _appendOutput = 0;
}

void
Command::print()
{
	printf("\n\n");
	printf("              COMMAND TABLE                \n");
	printf("\n");
	printf("  #   Simple Commands\n");
	printf("  --- ----------------------------------------------------------\n");
	
	for ( int i = 0; i < _numberOfSimpleCommands; i++ ) {
		printf("  %-3d ", i );
		for ( int j = 0; j < _simpleCommands[i]->_numberOfArguments; j++ ) {
			printf("\"%s\" \t", _simpleCommands[i]->_arguments[ j ] );
		}
        printf("\n");
	}

	printf( "\n\n" );
	printf( "  Output       Input        Error        Background\n" );
	printf( "  ------------ ------------ ------------ ------------\n" );
	printf( "  %-12s %-12s %-12s %-12s\n", _outFile?_outFile:"default",
		_inputFile?_inputFile:"default", _errFile?_errFile:"default",
		_background?"YES":"NO");
	printf( "\n\n" );
	
}

void
Command::execute()
{
	// Don't do anything if there are no simple commands
	if ( _numberOfSimpleCommands == 0 ) {
		prompt();
		return;
	}
    
    // exit command
    if (strcmp(_simpleCommands[0]->_arguments[0], "exit") == 0) {
        printf("  Exiting myshell...\n");
        exit(0);
    }

	// Print contents of Command data structure
    //print();

	// Add execution here
	// For every simple command fork a new process
	// Setup i/o redirection
	// and call exec
    int ret;
    int fdin;
    int fdout;

    int tempin = dup(0);
    int tempout = dup(1);
    int temperr = dup(2);

    if (_inputFile) {
        fdin = open(_inputFile, O_RDONLY | O_CREAT);
    } else {
        fdin = dup(tempin);
    }
    
    for (int i = 0; i < _numberOfSimpleCommands; i++) {
        dup2(fdin, 0);
        close(fdin);
        
        if (i == _numberOfSimpleCommands - 1) {
            if (_outFile) {
                if (_appendOutput) {
                    fdout = open(_outFile, O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
                } else {
                    fdout = open(_outFile, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
                }
            } else {
                fdout = dup(tempout);
            }

            if (_errFile) {
               dup2(fdout, 2); 
            }
        } else {
            int fdpipe[2];
            pipe(fdpipe);
            fdout = fdpipe[1];
            fdin = fdpipe[0];
        }

        dup2(fdout, 1);
        close(fdout);
        
        if (strcmp(_simpleCommands[0]->_arguments[0], "setenv") == 0) {
            setenv(_simpleCommands[0]->_arguments[1], _simpleCommands[0]->_arguments[2], 1);
        } else if (strcmp(_simpleCommands[0]->_arguments[0], "unsetenv") == 0) {
            unsetenv(_simpleCommands[0]->_arguments[1]);
        } else if (strcmp(_simpleCommands[0]->_arguments[0], "cd") == 0) {
            if (_simpleCommands[0]->_arguments[1] == NULL) {
                chdir(getenv("HOME"));
            } else {
                if (chdir(_simpleCommands[0]->_arguments[1]) < 0) {
                    perror("chdir");
                }
            }
        } else {
            ret = fork();
            if (ret == 0) {
                execvp(_simpleCommands[i]->_arguments[0], _simpleCommands[i]->_arguments);
                perror("execvp");
                _exit(1);
            } else if (ret < 0) {
                perror("fork");
                return;
            }
        }
    }

    dup2(tempin, 0);
    dup2(tempout, 1);
    dup2(temperr, 2);
    close(tempin);
    close(tempout);
    close(temperr);

    if (!_background) {
        waitpid(ret, NULL, 0);
    }

    if (ret == -1) _exit(0);

	// Clear to prepare for next command
	clear();
	
	// Print new prompt
	prompt();
}

// Shell implementation

void
Command::prompt()
{
    if (isatty(0)) {
	    printf("myshell>");
    }
	fflush(stdout);
}

Command Command::_currentCommand;
SimpleCommand * Command::_currentSimpleCommand;

int yyparse(void);

main()
{
    signal(SIGINT, ctrlc_handler);
    signal(SIGCHLD, zombie_handler);
	Command::_currentCommand.prompt();
	yyparse();
}

