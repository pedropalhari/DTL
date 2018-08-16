snazzle.l
%{
#include <cstdio>

#include "calculator.tab.h"  // to get the token types from Bison
#include <iostream>
using namespace std;

%}
%%
[ \t\n]           ;
[0-9]+\.[0-9]+    { yylval.fval = atof(yytext); return FLOAT; }
[0-9]+            { yylval.ival = atoi(yytext); return INT; }
[a-zA-Z0-9]+      {
	// We have to strdup because we can't rely on yytext not changing underneath us:
	yylval.sval = strdup(yytext);
	return STRING;
}
"+" {return SUM;}
"-" {return MINUS;}
"*" {return MUL;}
"/" {return DIV;}
"@" {return END;}
%%