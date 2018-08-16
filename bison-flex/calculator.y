%{
#include <cstdio>
#include <iostream>
using namespace std;

// Declare stuff from Flex that Bison needs to know about:
extern int yylex();
extern int yyparse();
extern FILE *yyin;
 
void yyerror(const char *s);
%}

// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  But tokens could be of any
// arbitrary data type!  So we deal with that in Bison by defining a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":
%union {
	int ival;
	float fval;
	char *sval;
}

%token SUM MINUS MUL DIV
%token END

// Define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING

%left END
%left SUM MINUS
%left MUL DIV

%type<ival> express

%%
// This is the actual grammar that bison will parse, but for right now it's just
// something silly to echo to the screen what bison gets from flex.  We'll
// make a real one shortly:

express: 
  INT {$$ = $1; cout << $$ << endl;} 
  | express DIV express {$$ = $1 / $3;}
  | express MUL express {$$ = $1 * $3;}
  | express SUM express {$$ = $1 + $3;}
  | express MINUS express {$$ = $1 - $3;} 
  | express END {cout << $1 << endl;}
  ;  
%%

int main(int, char**) {
	// Open a file handle to a particular file:
  cout << "COMEÇOU" << endl;
  while(yyparse());

/* 	FILE *myfile = fopen("a.snazzle.file", "r");
	// Make sure it is valid:
	if (!myfile) {
		cout << "I can't open a.snazzle.file!" << endl;
		return -1;
	}
	// Set Flex to read from it instead of defaulting to STDIN:
	yyin = myfile;
	
	// Parse through the input:
	yyparse(); */
	
}

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}