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
">=" {return GR_EQUAL;}
"<=" {return LE_EQUAL;}
"==" {return EQUALS;}
"!=" {return N_EQUALS;}
"&&" {return AND;}
"||" {return OR;}
"if" {return IF_S;}
"while" {return WHILE_S;}
"else" {return ELSE_S;}
"print" {return PRINT;}
"scan" {return SCAN;}
"fun" {return FUNC;}
[ \t\n]           ;
[0-9]+\.[0-9]+    { yylval.fval = atof(yytext); return FLOAT; }
[0-9]+            { yylval.ival = atoi(yytext); return INT; }
[a-zA-Z0-9]+      {
	// We have to strdup because we can't rely on yytext not changing underneath us:
	yylval.sval = strdup(yytext);
	return STRING;
}
\"(\\.|[^"\\])*\" {
	// We have to strdup because we can't rely on yytext not changing underneath us:
	yylval.sval = strdup(yytext);
	return STRING_Q;
}
%%