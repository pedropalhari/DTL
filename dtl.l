snazzle.l
%{
#include <unordered_map>
#include <vector>
#include <cstdio>
#include <iostream>
#include "dtl.tab.h"  // to get the token types from Bison

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
"\"" {return QMARKS;}
"=" {return ATTRIBUTION;}
"var " {return DECLARATION;}
"." {return DOT;}
";" {return ENDL;}
"," {return COMMA;}
"{" {return OPEN_CBRACKETS;}
"}" {return CLOSE_CBRACKETS;}
"(" {return OPEN_PAREN;}
")" {return CLOSE_PAREN;}
">" {return GREATER;}
"<" {return LESSER;}
"==" {return EQUALS;}
"!=" {return N_EQUALS;}
%%