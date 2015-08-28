
/*
 *
 * CS-252
 * shell.l: lexical analyzer for shell
 * You have to extend it.
 *
 */

%{

#include <string.h>
#include <unistd.h>
#include "y.tab.h"

/* Start raw mode */ 

extern "C" char * read_line();

int mygetc(FILE * f) {
    static char *p;
    char ch;

    if (!isatty(0)) {
        // stdin is not a tty. Call real getc
        return getc(f);
    }

    if (p == NULL || *p == 0) {
        char * s = read_line();
        p = s;
    }

    ch = *p;
    p++;

    return ch;
}

#undef getc
#define getc(f) mygetc(f)

/* End raw Mode */

static  void yyunput (int c,char *buf_ptr  );

void myunputc(int c) {
    unput(c);
}

%}

%%

\n 	    {
		    return NEWLINE;
	    }

[ \t] 	{
		    /* Discard spaces and tabs */
     	}	

">>&"	{
		    return GREATGREATAMPERSAND;
	    }

">&"	{
		    return GREATAMPERSAND;
	    }	

">>"	{
		    return GREATGREAT;
	    }

">"     {
		    return GREAT;
	    }

"<"	    {
		    return LESS;
	    }

"&"	    {
		    return AMPERSAND;
	    }

"|"     {
            return PIPE;
        }

\`[^\n]*\`    {
            yylval.string_val = strndup(yytext + 1, strlen(yytext) - 2);
            return SUBSHELL;
        }   

\"[^\n\"]*\"    {
            yylval.string_val = strndup(yytext + 1, strlen(yytext) - 2);
            return WORD;
        }

[^ |<>&\t\n]*[\\][^ \t\n]* {
            yylval.string_val = strdup(yytext);
            return ESCAPEWORD;
        }

[^ |<>&\t\n][^ |<>&\t\n]*  {
		    yylval.string_val = strdup(yytext);
		    return WORD;
	    }

