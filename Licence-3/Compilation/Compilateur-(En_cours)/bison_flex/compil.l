%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <limits.h>
  #include "../includes/type.h"
  #include "../includes/operand.h"
  #include "../includes/booleans.h"
  #include "../includes/hashtable/hashtable.h"
  #include "compil.tab.h"
  void string_to_int(int *r, const char *s);
  void yytext_copy(char **copy, const char *s);
%}

%option noyywrap

%%

0|[1-9][0-9]*                 {
                                string_to_int(&yylval.integer, yytext);
                                return NUM;
                              }
"+"|"-"|"*"|"/"|"("|")"       {return yytext[0];}
"<"|">"|"="                   {return yytext[0];}
"<="                          {return LE;}
">="                          {return GE;}

"true"                        {return TRUE;}
"false"                       {return FALSE;}

"&&"                          {return AND;}
"||"                          {return OR;}
"!"                           {return NOT;}
"^"                           {return XOR;}
"!="                          {return DIFF;}

"{"|"}"|","                   {return yytext[0];}
"\\begin{algo}"               {return B_ALGO;}
"\\end{algo}"                 {return E_ALGO;}
"\\RETURN"                    {return RET;}
"\\SET"                       {return SET;}
"\\DOFORI"                    {return FOR;}
"\\DOWHILE"                   {return WHILE;}
"\\CALL"                      {return CALL;}
"\\IF"                        {return IF;}
"\\ELSE"                      {return ELSE;}
"\\FI"                        {return FI;}
"\\OD"                        {return OD;}

[a-zA-Z_][a-zA-Z0-9_]*        {yytext_copy(&yylval.string, yytext); return ID;}

[[:blank:]]|[[:space:]]       {}
.                             {printf("Bad character: %c\n", yytext[0]); yyterminate();}    
%%


void string_to_int(int *r, const char *s) {
  char *p;
  long v;
  errno = 0;
  v = strtol(s, &p, 10);
  if ( ( *p != '\0' || ( errno == ERANGE 
                     && ( v == LONG_MIN || v == LONG_MAX ) ) ) 
       || ( v < INT_MIN || v > INT_MAX ) ) {
    fprintf(stderr, "Error converting string to int\n");
    exit(EXIT_FAILURE);
  } 
  *r = v;
}

void yytext_copy(char **id, const char *s) {
  char *copy = malloc(strlen(s) + 1);
  if (copy == NULL) {
    fprintf(stderr, "Memory allocation problem.\n");
    exit(EXIT_FAILURE);
  }
  strcpy(copy,s);
  *id = copy;
}

