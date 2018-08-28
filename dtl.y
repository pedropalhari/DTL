%{
#include <cstdio>
#include <iostream>
#include <string>
#include <unordered_map>
#include <any>

using namespace std;

// Declare stuff from Flex that Bison needs to know about:
extern int yylex();
extern int yyparse();
extern FILE *yyin;
 
void yyerror(const char *s);

typedef struct{
  int type;
  void *ptrToData;
} basicType;

unordered_map<string, int> GLOBAL;

%}

%union {
	int ival;
	float fval;
	char *sval;
}

%token DECLARATION ATTRIBUTION DOT


// Define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING

%%
// This is the actual grammar that bison will parse, but for right now it's just
// something silly to echo to the screen what bison gets from flex.  We'll
// make a real one shortly:

var:
  DECLARATION STRING {
    GLOBAL[$2] = 0;

    cout << $2 << ' ' << GLOBAL[$2] << endl;
  }

%%

int main(int, char *argv[]) {
  //TEST AREA
  unordered_map<string, any> teste;
  teste["a"] = 1;

	FILE *myfile = fopen(argv[1], "r");
	if (!myfile) {
		cout << "No file recognized." << endl;
		return -1;
	}
	// Set Flex to read from it instead of defaulting to STDIN:
	yyin = myfile;
	
	// Parse through the input:
	yyparse(); 	
}

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}