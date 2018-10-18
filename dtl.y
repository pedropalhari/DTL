%{
#include <unordered_map>
#include <vector>
#include <cstdio>
#include <iostream>
#include <string>
#include <any>
#include <functional>
#define cast(X) (*any_cast<unordered_map<string, basicObject>*>(X.obj))

using namespace std;

// Declare stuff from Flex that Bison needs to know about:
extern int yylex();
extern int yyparse();
extern FILE *yyin;
 
void yyerror(const char *s);

//LANGUAGE BACKEND
enum Type { Object, Integer, Float, String, Function };
typedef struct {
  Type type;
  any obj;
} basicObject;

unordered_map<string, basicObject> *auxForObjectInitialization = new unordered_map<string, basicObject>();
vector<string> auxForCascadedObjects = vector<string>();

int shouldExecute = 1;

string anyTypeString = "NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE";

basicObject GLOBAL;
//Vetor de funções lambda de instruções pro programa
vector<function<void()>> runProgram;
vector<function<void()>>::iterator globalProgramIterator;

//Stack para identificar blocos de função if
vector<long> ifHeadStack;
vector<long> ifFootStack;

//Uma stack para resolver expressões, como uma calculadora. Também resolve > < == etc
vector<any> globalExpressionStack;

void decast(basicObject x) {
  if (x.type == Integer) cout << any_cast<int>(x.obj) << endl;
	if (x.type == String) cout << any_cast<string>(x.obj) << endl;
}

void attr(basicObject &var, basicObject attribute){
	var = attribute;
}

void checkIf(int ifHead, int ifFoot, int condition){
	cout << "TESTANDO IF: " << ifHead << " " << ifFoot << " " << condition << endl;
	if(condition) return;
	else globalProgramIterator += ifFoot - ifHead; //Pula o corpo do if
}

// 0 = +, 1 = -, 2 = *, 3 = /
// 4 = >, 5 = <, 6 = >=, 7 = <=
// 8 = ==, 9 = !=
void doOperationWithExpression(int operationType){
	cout << "found sum " << globalExpressionStack.size() << endl;
	any secondExpress = globalExpressionStack.back();
	globalExpressionStack.pop_back();
	any firstExpress = globalExpressionStack.back();
	globalExpressionStack.pop_back();

	if(anyTypeString.compare(firstExpress.type().name()) == 0){
		cout << "Variable in Expression (1): " << any_cast<string>(firstExpress) << endl;

		cout <<  any_cast<int> ((cast(GLOBAL)[any_cast<string>(firstExpress)]).obj) << endl;
		firstExpress = ((cast(GLOBAL)[any_cast<string>(firstExpress)]).obj);
	}

	if(anyTypeString.compare(secondExpress.type().name()) == 0){
		cout << "Variable in Expression (2): " << any_cast<string>(secondExpress) << endl;

		cout <<  any_cast<int> ((cast(GLOBAL)[any_cast<string>(secondExpress)]).obj) << endl;
		secondExpress = ((cast(GLOBAL)[any_cast<string>(secondExpress)]).obj);
	}

	int result;

	switch(operationType){
		case 0:
			result = any_cast<int>(firstExpress) + any_cast<int>(secondExpress);
			break;
		
		case 1:
			result = any_cast<int>(firstExpress) - any_cast<int>(secondExpress);
			break;

		case 2:
			result = any_cast<int>(firstExpress) * any_cast<int>(secondExpress);
			break;

		case 3:
			result = any_cast<int>(firstExpress) / any_cast<int>(secondExpress);
			break;

		case 4:
			result = any_cast<int>(firstExpress) > any_cast<int>(secondExpress);
			break;

		case 5:
			result = any_cast<int>(firstExpress) < any_cast<int>(secondExpress);
			break;

		case 6:
			result = any_cast<int>(firstExpress) >= any_cast<int>(secondExpress);
			break;

		case 7:
			result = any_cast<int>(firstExpress) <= any_cast<int>(secondExpress);
			break;

		case 8:
			result = any_cast<int>(firstExpress) == any_cast<int>(secondExpress);
			break;

		case 9:
			result = any_cast<int>(firstExpress) != any_cast<int>(secondExpress);
			break;
	}
	
	globalExpressionStack.push_back(result);
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

%type <sval> printVar;

%type <ival> ifhead

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
%token IF_S ELSE_S WHILE_S
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
		auxObject.obj = 0;
		string varName = $2;

		runProgram.push_back([auxObject, varName]() { 
			attr(cast(GLOBAL)[varName], auxObject); 
			cout << "VARIABLE DECLARATION: " << varName << endl;	
		});		
  }
	;

varAttribution:
 STRING ATTRIBUTION express ENDL {
		string varName = $1;

		runProgram.push_back([varName]() { 
			basicObject auxObject;
			auxObject.type = Integer;
			auxObject.obj = globalExpressionStack.back();
			globalExpressionStack.pop_back();

			attr(cast(GLOBAL)[varName], auxObject); 
			cout << "VARIABLE ATTRIBUTION: (INTEGER) " << varName << " = ";
			decast(cast(GLOBAL)[varName]);	 	
		});				
		
	}
	|	STRING ATTRIBUTION STRING_Q ENDL {
		
		basicObject auxObject;
		auxObject.type = String;
		string aux2 = $3;
		auxObject.obj = aux2;
		
		string varName = $1;

		runProgram.push_back([auxObject, varName]() { 
			attr(cast(GLOBAL)[varName], auxObject); 
			cout << "VARIABLE ATTRIBUTION: (STRING) " << varName << " = ";
			decast(cast(GLOBAL)[varName]);	 	
		});				
	}
	|	STRING ATTRIBUTION object ENDL {
		
		basicObject auxObject;
		auxObject.type = Object;
		auxObject.obj = (unordered_map<string, basicObject> *) $3;
		
		string varName = $1;

		runProgram.push_back([auxObject, varName]() { 
			attr(cast(GLOBAL)[varName], auxObject); 

			cout << "VARIABLE ATTRIBUTION: (OBJECT) " << varName << " = {" << endl;

			for(auto x : cast(cast(GLOBAL)[varName])){
				cout << "\t" << x.first << " = ";
				decast(x.second);
			}
			cout << "}" << endl;
		});
	}
	;

cascadedRef:
	STRING DOT STRING { auxForCascadedObjects.push_back($1); auxForCascadedObjects.push_back($3);}
	| cascadedRef DOT STRING {auxForCascadedObjects.push_back($3);}
	;

varAttributionCascaded:
 cascadedRef ATTRIBUTION express ENDL {
		vector<string> localCascadedObjects = auxForCascadedObjects;

		runProgram.push_back([localCascadedObjects]() { 			
			basicObject auxObject;
			auxObject.type = Integer;
			auxObject.obj = globalExpressionStack.back();
			globalExpressionStack.pop_back();


			string stringForPrintingLater;
			basicObject auxDereference = GLOBAL;
			for(auto x : localCascadedObjects){
				if(x == localCascadedObjects.back()){ //Se for o ultimo elemento rola a atribuição nele
					attr(cast(auxDereference)[x], auxObject); 
					
					stringForPrintingLater = x;				
				} else { //Se não vai descendo recursivamente
					auxDereference = cast(auxDereference)[x];
				}
			}

			cout << "VARIABLE ATTRIBUTION: (INTEGER) ";
			
			for(auto x : localCascadedObjects) 
				if(x == localCascadedObjects.front())
					cout << x;
				else cout << "." << x;

			cout << " = ";

			decast(cast(auxDereference)[stringForPrintingLater]);
		});

		auxForCascadedObjects.clear();		
	}
	| cascadedRef ATTRIBUTION STRING_Q ENDL {
		
		basicObject auxObject;
		auxObject.type = String;
		string aux2 = $3;
		auxObject.obj = aux2;

		vector<string> localCascadedObjects = auxForCascadedObjects;

		runProgram.push_back([auxObject, localCascadedObjects]() { 			
			string stringForPrintingLater;
			basicObject auxDereference = GLOBAL;
			for(auto x : localCascadedObjects){
				if(x == localCascadedObjects.back()){ //Se for o ultimo elemento rola a atribuição nele
					attr(cast(auxDereference)[x], auxObject); 
					
					stringForPrintingLater = x;				
				} else { //Se não vai descendo recursivamente
					auxDereference = cast(auxDereference)[x];
				}
			}

			cout << "VARIABLE ATTRIBUTION: (STRING) ";
			
			for(auto x : localCascadedObjects) 
				if(x == localCascadedObjects.front())
					cout << x;
				else cout << "." << x;

			cout << " = ";

			decast(cast(auxDereference)[stringForPrintingLater]);
		});

		auxForCascadedObjects.clear();
	}
	| cascadedRef ATTRIBUTION object ENDL {
		
		basicObject auxObject;
		auxObject.type = Object;
		auxObject.obj = (unordered_map<string, basicObject> *) $3;

		vector<string> localCascadedObjects = auxForCascadedObjects;

		runProgram.push_back([auxObject, localCascadedObjects]() { 			
			string stringForPrintingLater;
			basicObject auxDereference = GLOBAL;
			for(auto x : localCascadedObjects){
				if(x == localCascadedObjects.back()){ //Se for o ultimo elemento rola a atribuição nele
					attr(cast(auxDereference)[x], auxObject); 
					
					stringForPrintingLater = x;				
				} else { //Se não vai descendo recursivamente
					auxDereference = cast(auxDereference)[x];
				}
			}

			cout << "VARIABLE ATTRIBUTION: (OBJECT) ";
			
			for(auto x : localCascadedObjects) 
				if(x == localCascadedObjects.front())
					cout << x;
				else cout << "." << x;

			cout << " = {" << endl;

			for(auto x : cast(cast(auxDereference)[stringForPrintingLater])){
				cout << "\t" << x.first << " = ";
				decast(x.second);
			}
			cout << "}" << endl;

			//decast((cast(auxDereference))[stringForPrintingLater]);
		});

		auxForCascadedObjects.clear();		
	}
	;

//CALCULATOR
express: 
  INT {
		//$$ = $1;

			int numberFound = $1;

			runProgram.push_back([numberFound]() {
				cout << "found int " << globalExpressionStack.size() << endl;
				globalExpressionStack.push_back(numberFound);
			});
			
		} 
	| STRING {
			string varName = $1;

			runProgram.push_back([varName]() {
				cout << "found var " << globalExpressionStack.size() << endl;
				globalExpressionStack.push_back(varName);
			});
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
  | express DIV express {
				//$$ = $1 / $3;
				
				runProgram.push_back([](){
					doOperationWithExpression(3);
				}); 
			}
  | express MUL express {
			//$$ = $1 * $3;
			runProgram.push_back([](){
					doOperationWithExpression(2);
			}); 
		}
  | express SUM express {
			//$$ = $1 + $3;
			runProgram.push_back([](){
				doOperationWithExpression(0);
			}); 	
		}
  | express MINUS express {
		//$$ = $1 - $3;
		runProgram.push_back([](){
				doOperationWithExpression(1);
		}); 	
	}
	| express GREATER express {
		runProgram.push_back([](){
				doOperationWithExpression(4);
		}); 
	}
	| express LESSER express {
		runProgram.push_back([](){
				doOperationWithExpression(5);
		}); 
	}
	| express GR_EQUAL express {
		runProgram.push_back([](){
				doOperationWithExpression(6);
		}); 
	}
	| express LE_EQUAL express {
		runProgram.push_back([](){
				doOperationWithExpression(7);
		}); 
	}
	| express EQUALS express {
		runProgram.push_back([](){
				doOperationWithExpression(8);
		}); 
	}
	| express N_EQUALS express {
		runProgram.push_back([](){
				doOperationWithExpression(9);
		}); 
	}
	| OPEN_PAREN express CLOSE_PAREN {
		// Tá certo, tem que parsear os internos primeiros
		// então fica essa declaração para dar prioridade na hora de jogar
		// nas respectivas stacks
	}
  ;  

codeblock:
	 OPEN_CBRACKETS body CLOSE_CBRACKETS;

ifhead:
	IF_S OPEN_PAREN express CLOSE_PAREN { //Reconhece esse padrão quando começa o if
		ifHeadStack.push_back(runProgram.size());
		$$ = $3; 
	};

elsehead:
	ELSE_S { shouldExecute = !shouldExecute;} //Else roda se o if não rodar, vice-versa
	;

ifelse:
	ifhead codeblock { 
		//Reconhece esse padrão quando acaba o if
		ifFootStack.push_back(runProgram.size()); 

		cout << "IF HEAD STACK: ";
		for(auto x : ifHeadStack)
			cout << x << " ";
		cout << endl;

		cout << "IF FOOT STACK: ";
		for(auto x : ifFootStack)
			cout << x << " ";
		cout << endl;

		cout << "FECHANDO UM IF:";
		int ifHead = ifHeadStack.back();
		int ifFoot = ifFootStack.back();

		ifHeadStack.pop_back();
		ifFootStack.pop_back();

		cout << ifHead << " " << ifFoot << endl;

		// AQUI EU COLOCO UMA FUNÇÃO NO RUNPROGRAM QUE FAZ O CHEQUE DOS IFS, ESSA FUNCAO FICA 
		// NA POSICAO IFHEAD (INSERT), COMPARA O STATEMENT E CONTINUA OU PULA A EXECUÇÃO PRO
		// IFFOOT
		
		//runProgram checkIf(ifHead, ifFoot, $1); falta dar o Insert e jogar no runprogram
		int condition = $1;
		vector<function<void()>>::iterator ifHeadPositionIterator = runProgram.begin() + ifHead;
		runProgram.insert(ifHeadPositionIterator, [ifHead, ifFoot, condition]() { 
			int conditionResult = any_cast<int>(globalExpressionStack.back()); //O resultado da expressão está no final do Expression Stack
			globalExpressionStack.pop_back();

			checkIf(ifHead, ifFoot, conditionResult); 
			cout << "CHECKING IF: " << conditionResult << endl;	
		});	

	} //rola depois que eu passo pelo ifhead
	| ifhead codeblock elsehead codeblock { shouldExecute = 1;}; //rola depois que eu passo pelo ifhead
	;


printVar:
	STRING {
		string varName = $1;
		runProgram.push_back([varName]() {
			cout << varName << " = ";
			decast(cast(GLOBAL)[varName]);
		});}
	| printVar COMMA STRING {
		string varName = $3;
		runProgram.push_back([varName]() {
			cout << varName << " = ";
			decast(cast(GLOBAL)[varName]);
		});}
	;

print:
	PRINT OPEN_PAREN printVar CLOSE_PAREN ENDL
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

	int i = 0;
	for(globalProgramIterator = runProgram.begin(); globalProgramIterator != runProgram.end(); ++globalProgramIterator){
		//cout << i++ << endl;
		(*globalProgramIterator)();

		for(auto i : globalExpressionStack)
			if(anyTypeString.compare(i.type().name()) == 0)
				cout << any_cast<string>(i) << ", ";
			else
				cout << any_cast<int>(i) << ", ";
		cout << endl;
	}
}

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}