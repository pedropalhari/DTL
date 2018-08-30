%{
#include <unordered_map>
#include <vector>
#include <cstdio>
#include <iostream>
#include <string>
#include <any>
#define cast(X) (*any_cast<unordered_map<string, basicObject>*>(X.obj))

using namespace std;

// Declare stuff from Flex that Bison needs to know about:
extern int yylex();
extern int yyparse();
extern FILE *yyin;
 
void yyerror(const char *s);

//LANGUAGE BACKEND
enum Type { Object, Integer, Float, String };
typedef struct {
  Type type;
  any obj;
} basicObject;

unordered_map<string, basicObject> *auxForObjectInitialization = new unordered_map<string, basicObject>();
vector<string> auxForCascadedObjects = vector<string>();

int shouldExecute = 1;

basicObject GLOBAL;

void decast(basicObject x) {
  if (x.type == Integer) cout << any_cast<int>(x.obj) << endl;
	if (x.type == String) cout << any_cast<string>(x.obj) << endl;
}

%}

%union {
	int ival;
	float fval;
	char *sval;
	void *oval; //Object
}

%type<ival> express

%type <oval> object;
%type <oval> objectDeclarationBody;

%type <oval> cascadedRef;

%type <ival> statement;

%type <sval> printVar;

// Define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING
%token <sval> STRING_Q

%token ENDL
%token DECLARATION QMARKS ATTRIBUTION DOT
%token OPEN_CBRACKETS CLOSE_CBRACKETS COMMA OPEN_PAREN CLOSE_PAREN
%token GREATER LESSER EQUALS N_EQUALS GR_EQUAL LE_EQUAL
%token IF_S ELSE_S
%token PRINT

%token SUM MINUS MUL DIV
%left SUM MINUS
%left MUL DIV
%left COMMA
%left GREATER LESSER EQUALS N_EQUALS GR_EQUAL LE_EQUAL

%%
// This is the actual grammar that bison will parse, but for right now it's just
// something silly to echo to the screen what bison gets from flex.  We'll
// make a real one shortly:

wholeProgram:
	body {cout << "FINISHED!" << endl;}

body:
	| varDeclaration body
	| varAttribution body
	| varAttributionCascaded body
	| ifelse body
	| print body
	;

//O QUE ACONTECE AQUI É QUE EU USO UM OBJETO GLOBAL PRA 
//GERAR UM UNORDERED_MAP QUE ALOCA AS PROPRIEDADES DE UM OBJETO
//E DEPOIS "RETORNO" ELE PRA ATRIBUIÇÃO DO OBJETO LÁ EM x = {}
objectDeclarationBody:
	DECLARATION STRING {
			basicObject basicObjectCreated;
			basicObjectCreated.type = Integer;
			basicObjectCreated.obj = 0;

			(*auxForObjectInitialization)[$2] = basicObjectCreated;
			
			$$ = auxForObjectInitialization;
		}
	| objectDeclarationBody COMMA objectDeclarationBody
	;

object:
	OPEN_CBRACKETS objectDeclarationBody CLOSE_CBRACKETS {
		unordered_map<string, basicObject> *aux = (unordered_map<string, basicObject> *) $2;
		$$ = aux;

		//Limpa o objeto no final, porque o objeto criado já foi passado para ser assimilado
		auxForObjectInitialization = new unordered_map<string, basicObject>();
		}
	;

// VARIABLE ASSIGNING
varDeclaration:
 DECLARATION STRING ENDL {
		basicObject auxObject;
		auxObject.type = Integer;
		auxObject.obj = 3;
		cast(GLOBAL)[$2] = auxObject;

		cout << "VARIABLE DECLARATION: " << $2 << endl;
  }
	;

varAttribution:
 STRING ATTRIBUTION statement ENDL {
	 if(shouldExecute){
		basicObject auxObject;
		auxObject.type = Integer;
		auxObject.obj = $3;

		cast(GLOBAL)[$1] = auxObject;

		cout << "VARIABLE ATTRIBUTION: (INTEGER) " << $1 << " = ";

		decast(cast(GLOBAL)[$1]);	 
	}}
	|	STRING ATTRIBUTION STRING_Q ENDL {
		if(shouldExecute){
		basicObject auxObject;
		auxObject.type = String;
		string aux2 = $3;
		auxObject.obj = aux2;
		cast(GLOBAL)[$1] = auxObject;

		cout << "VARIABLE ATTRIBUTION: (STRING) " << $1 << " = ";

		decast(cast(GLOBAL)[$1]);
	}}
	|	STRING ATTRIBUTION object ENDL {
		if(shouldExecute){
		basicObject auxObject;
		auxObject.type = Object;
		auxObject.obj = (unordered_map<string, basicObject> *) $3;
		
		cast(GLOBAL)[$1] = auxObject;

		cout << "VARIABLE ATTRIBUTION: (OBJECT) " << $1 << " = {" << endl;

		for(auto x : cast(cast(GLOBAL)[$1])){
			cout << "\t" << x.first << " = ";
			decast(cast(cast(GLOBAL)[$1])[x.first]);
		}
		cout << "}" << endl;
	}}
	;

cascadedRef:
	STRING DOT STRING { auxForCascadedObjects.push_back($1); auxForCascadedObjects.push_back($3);}
	| cascadedRef DOT STRING {auxForCascadedObjects.push_back($3);}
	;

varAttributionCascaded:
 cascadedRef ATTRIBUTION statement ENDL {
	 if(shouldExecute){
		basicObject auxObject;
		auxObject.type = Integer;
		auxObject.obj = $3;

		string stringForPrintingLater;
		basicObject auxDereference = GLOBAL;
		for(auto x : auxForCascadedObjects){
			if(x == auxForCascadedObjects.back()){ //Se for o ultimo elemento rola a atribuição nele
				cast(auxDereference)[x] = auxObject;

				
				stringForPrintingLater = x;				
			} else { //Se não vai descendo recursivamente
				auxDereference = cast(auxDereference)[x];
			}
		}

		cout << "VARIABLE ATTRIBUTION: (INTEGER) ";
		
		for(auto x : auxForCascadedObjects) 
			if(x == auxForCascadedObjects.front())
				cout << x;
			else cout << "." << x;

		cout << " = ";

		decast(cast(auxDereference)[stringForPrintingLater]);

		auxForCascadedObjects.clear();		
	}}
	| cascadedRef ATTRIBUTION STRING_Q ENDL {
		if(shouldExecute){
		basicObject auxObject;
		auxObject.type = String;
		string aux2 = $3;
		auxObject.obj = aux2;

		string stringForPrintingLater;
		basicObject auxDereference = GLOBAL;
		for(auto x : auxForCascadedObjects){
			if(x == auxForCascadedObjects.back()){ //Se for o ultimo elemento rola a atribuição nele
				cast(auxDereference)[x] = auxObject;
				stringForPrintingLater = x;				
			} else { //Se não vai descendo recursivamente
				auxDereference = cast(auxDereference)[x];
			}
		}

		decast(cast(auxDereference)[stringForPrintingLater]);

		auxForCascadedObjects.clear();
	}}
	| cascadedRef ATTRIBUTION object ENDL {
		if(shouldExecute){
		basicObject auxObject;
		auxObject.type = Object;
		auxObject.obj = (unordered_map<string, basicObject> *) $3;

		string stringForPrintingLater;
		basicObject auxDereference = GLOBAL;
		for(auto x : auxForCascadedObjects){
			if(x == auxForCascadedObjects.back()){ //Se for o ultimo elemento rola a atribuição nele
				cast(auxDereference)[x] = auxObject;
				stringForPrintingLater = x;				
			} else { //Se não vai descendo recursivamente
				auxDereference = cast(auxDereference)[x];
			}
		}

		decast(cast(auxDereference)[stringForPrintingLater]);

		auxForCascadedObjects.clear();		
	}}
	;

//CALCULATOR
express: 
  INT {$$ = $1;} 
	| STRING {
			any aux = (cast(GLOBAL)[$1]);
			$$ = any_cast<int> ((cast(GLOBAL)[$1]).obj);
		}
	| cascadedRef {
			basicObject auxDereference = GLOBAL;
			for(auto x : auxForCascadedObjects){
				if(x == auxForCascadedObjects.back()){ //Se for o ultimo elemento rola a atribuição nele
					$$ = any_cast<int> ((cast(auxDereference)[x]).obj);				
				} else { //Se não vai descendo recursivamente
					auxDereference = cast(auxDereference)[x];
				}
		}

		auxForCascadedObjects.clear();
	}
  | express DIV express {$$ = $1 / $3;}
  | express MUL express {$$ = $1 * $3;}
  | express SUM express {$$ = $1 + $3;}
  | express MINUS express {$$ = $1 - $3;}
	| OPEN_PAREN express CLOSE_PAREN {$$ = $2;}
  ;  

//CONDITIONS
statement:
	express {$$ = $1;}
	| statement GREATER statement {$$ = $1 > $3;}
	| statement LESSER statement {$$ = $1 < $3;}
	| statement GR_EQUAL statement {$$ = $1 >= $3;}
	| statement LE_EQUAL statement {$$ = $1 <= $3;}
	| statement EQUALS statement {$$ = $1 == $3;}
	| statement N_EQUALS statement {$$ = $1 != $3;}
	| OPEN_PAREN statement CLOSE_PAREN {$$ = $2;}
	;

codeblock:
	 OPEN_CBRACKETS body CLOSE_CBRACKETS;

ifhead:
	IF_S OPEN_PAREN statement CLOSE_PAREN { if($3) shouldExecute = 1; else shouldExecute = 0;};

elsehead:
	ELSE_S { shouldExecute = !shouldExecute;} //Else roda se o if não rodar, vice-versa

ifelse:
	 ifhead codeblock { shouldExecute = 1;} //rola depois que eu passo pelo ifhead
	 | ifhead codeblock elsehead codeblock { shouldExecute = 1;}; //rola depois que eu passo pelo ifhead

printVar:
	STRING {decast(cast(GLOBAL)[$1]);}
	| printVar COMMA STRING {decast(cast(GLOBAL)[$3]);}
	;

print:
	PRINT OPEN_PAREN printVar CLOSE_PAREN {}
	;
%%

int main(int, char *argv[]) {
	//ENV SETUP
	GLOBAL.type = Object;
	GLOBAL.obj = new unordered_map<string, basicObject>();

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