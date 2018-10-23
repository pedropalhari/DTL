/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "dtl.y" /* yacc.c:339  */

#include <unordered_map>
#include <vector>
#include <cstdio>
#include <iostream>
#include <string>
#include <any>
#include <functional>
#include <stdlib.h> //Exit
#define cast(X) (*any_cast<unordered_map<string, basicObject>*>(X.obj))
#define DEBUG_MODE false

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

//Stack para identificar blocos de função while
vector<long> whileHeadStack;
vector<long> whileBodyStack;
vector<long> whileFootStack;

//Stack para identificar blocos de função 
unordered_map<string, long> functionMap; //Pra chamada de função
vector<vector<function<void()>>::iterator> functionReturnStack; //Pro retorno da função
vector<long> functionHeadStack;
vector<long> functionFootStack;


//Uma stack para resolver expressões, como uma calculadora. Também resolve > < == etc
vector<any> globalExpressionStack;
vector<any> globalExpressionStackDuplicate; //Usado pois o while precisa ficar reavaliando a mesma expressão
vector<function<void()>> globalOperationStackDuplicate; //Usado pois o while precisa ficar reavaliando a mesma expressão (operacoes)
int shouldDuplicateExpresssionStack = false;

void decast(basicObject x) {
  if (x.type == Integer) cout << any_cast<int>(x.obj) << endl;
	if (x.type == String) cout << any_cast<string>(x.obj) << endl;
}

void attr(basicObject &var, basicObject attribute){
	var = attribute;
}

void checkIf(int ifHead, int ifFoot, int condition){
	if(DEBUG_MODE){
		cout << "TESTANDO IF: " << ifHead << " " << ifFoot << " " << condition << endl;
	}

	if(condition){
		if(DEBUG_MODE)
			cout << "IF INDO PARA A PROX INSTRUÇÃO" << endl;
		return;
	}
	else {
		if(DEBUG_MODE)
			cout << "IF INDO PARA " << ifFoot - ifHead << endl; 
		globalProgramIterator += ifFoot - ifHead; //Pula o corpo do if
	}
}

// 0 = +, 1 = -, 2 = *, 3 = /
// 4 = >, 5 = <, 6 = >=, 7 = <=
// 8 = ==, 9 = !=, 10 = &&, 11 = ||
void doOperationWithExpression(int operationType){
	any secondExpress = globalExpressionStack.back();
	globalExpressionStack.pop_back();
	any firstExpress = globalExpressionStack.back();
	globalExpressionStack.pop_back();

	//Não precisa mais, o decast acontece no push pra stack
	/* if(anyTypeString.compare(firstExpress.type().name()) == 0){
		if(DEBUG_MODE){
			cout << "Variable in Expression (1): " << any_cast<string>(firstExpress) << endl;
			cout <<  any_cast<int> ((cast(GLOBAL)[any_cast<string>(firstExpress)]).obj) << endl;
		}

		firstExpress = ((cast(GLOBAL)[any_cast<string>(firstExpress)]).obj);
	}

	if(anyTypeString.compare(secondExpress.type().name()) == 0){
		if(DEBUG_MODE){
			cout << "Variable in Expression (2): " << any_cast<string>(secondExpress) << endl;
			cout <<  any_cast<int> ((cast(GLOBAL)[any_cast<string>(secondExpress)]).obj) << endl;
		}

		secondExpress = ((cast(GLOBAL)[any_cast<string>(secondExpress)]).obj);
	} */

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

		case 10:
			result = any_cast<int>(firstExpress) && any_cast<int>(secondExpress);
			break;

		case 11:
			result = any_cast<int>(firstExpress) || any_cast<int>(secondExpress);
			break;
	}
	
	globalExpressionStack.push_back(result);
}


#line 239 "dtl.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "dtl.tab.h".  */
#ifndef YY_YY_DTL_TAB_H_INCLUDED
# define YY_YY_DTL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    FLOAT = 259,
    STRING = 260,
    STRING_Q = 261,
    ENDL = 262,
    DECLARATION = 263,
    QMARKS = 264,
    ATTRIBUTION = 265,
    DOT = 266,
    OPEN_CBRACKETS = 267,
    CLOSE_CBRACKETS = 268,
    COMMA = 269,
    OPEN_PAREN = 270,
    CLOSE_PAREN = 271,
    GREATER = 272,
    LESSER = 273,
    EQUALS = 274,
    N_EQUALS = 275,
    GR_EQUAL = 276,
    LE_EQUAL = 277,
    AND = 278,
    OR = 279,
    IF_S = 280,
    ELSE_S = 281,
    WHILE_S = 282,
    FUNC = 283,
    PRINT = 284,
    SCAN = 285,
    SUM = 286,
    MINUS = 287,
    MUL = 288,
    DIV = 289
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 174 "dtl.y" /* yacc.c:355  */

	int ival;
	float fval;
	char *sval;
	void *oval; //Object

#line 321 "dtl.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_DTL_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 338 "dtl.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  32
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   203

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  56
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  123

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   220,   220,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   241,   250,   254,   265,   281,   299,   317,
     342,   346,   352,   390,   428,   475,   494,   507,   539,   551,
     557,   563,   569,   574,   579,   584,   589,   594,   599,   604,
     609,   617,   620,   625,   671,   677,   683,   707,   717,   721,
     739,   780,   786,   828,   846,   856,   862
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "FLOAT", "STRING", "STRING_Q",
  "ENDL", "DECLARATION", "QMARKS", "ATTRIBUTION", "DOT", "OPEN_CBRACKETS",
  "CLOSE_CBRACKETS", "COMMA", "OPEN_PAREN", "CLOSE_PAREN", "GREATER",
  "LESSER", "EQUALS", "N_EQUALS", "GR_EQUAL", "LE_EQUAL", "AND", "OR",
  "IF_S", "ELSE_S", "WHILE_S", "FUNC", "PRINT", "SCAN", "SUM", "MINUS",
  "MUL", "DIV", "$accept", "wholeProgram", "body", "objectDeclarationBody",
  "object", "varDeclaration", "varAttribution", "cascadedRef",
  "varAttributionCascaded", "express", "codeblock", "ifhead", "ifelse",
  "printVar", "print", "scan", "functionHead", "function", "funCall",
  "whileAtomic", "whileHead", "while", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289
};
# endif

#define YYPACT_NINF -29

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-29)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       1,    22,    -3,     7,   -29,    35,    29,    50,    67,   -29,
       1,     1,    36,     1,    65,     1,     1,     1,    65,     1,
       1,    63,    65,     1,    33,    74,    76,    73,    -2,   -29,
      44,    85,   -29,   -29,   -29,   180,    88,   -29,     1,   -29,
     -29,   -29,   -29,   -29,   -29,   -29,    -2,   -29,   -29,   -29,
      83,   100,   102,    -2,   104,    97,    64,   -29,   105,   -29,
     101,    83,   -29,    97,    27,    -6,     0,   119,   121,    82,
     -29,    96,   120,   -29,   122,    61,   139,   -29,   -29,    -2,
      -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,
      -2,   -29,   -29,   124,   123,   138,   140,   -29,   -29,   -29,
     -29,   -29,   -29,   -29,   102,   -29,   -29,   -29,   -29,   -29,
     -29,   -29,   -29,   -29,   157,   157,   179,   179,   -29,   -29,
     -29,   -29,   -29
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     0,    54,     0,     0,     0,     0,     2,
       3,     3,     0,     3,     0,     3,     3,     3,     0,     3,
       3,     0,     0,     3,     0,     0,     0,     0,     0,    51,
       0,     0,     1,     4,     5,     0,     0,     6,     3,    43,
       7,     8,     9,    52,    10,    11,     0,    56,    12,    25,
      26,     0,     0,     0,     0,    27,     0,    20,     0,    16,
       0,    44,    45,    46,     0,     0,     0,     0,     0,     0,
      21,     0,     0,    18,     0,     0,     0,    19,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    42,     0,     0,     0,     0,    23,    24,    22,
      41,    55,    13,    15,     0,    40,    32,    33,    36,    37,
      34,    35,    38,    39,    30,    31,    29,    28,    47,    48,
      49,    50,    14
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -29,   -29,    53,    42,   113,   -29,   -29,     4,   -29,   -28,
     -10,   -29,   -29,   -29,   -29,   -29,   -29,   -29,   -29,   -29,
     -29,   -29
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     8,     9,    75,    54,    10,    11,    55,    13,    56,
      39,    14,    15,    64,    16,    17,    18,    19,    20,    21,
      22,    23
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      60,    49,    27,    50,    12,    25,     1,    69,    43,     2,
      95,    36,    47,    53,    12,    12,    96,    12,    72,    12,
      12,    12,    28,    12,    12,    76,     3,    12,     4,     5,
       6,     7,    24,    25,    63,    66,    49,    26,    50,    51,
      29,    93,    12,    94,    30,    52,    35,    36,    53,    61,
      62,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    33,    34,    31,    37,    32,    40,    41,
      42,    78,    44,    45,   103,   104,    48,    38,    46,    57,
      59,    79,    80,    81,    82,    83,    84,    85,    86,    99,
      65,    71,    58,    70,    25,    87,    88,    89,    90,    79,
      80,    81,    82,    83,    84,    85,    86,    73,    36,   100,
      74,    77,    91,    87,    88,    89,    90,    92,    79,    80,
      81,    82,    83,    84,    85,    86,    97,   102,    98,   118,
     119,     0,    87,    88,    89,    90,   101,    79,    80,    81,
      82,    83,    84,    85,    86,   120,   122,   121,    68,     0,
       0,    87,    88,    89,    90,   105,    79,    80,    81,    82,
      83,    84,    85,    86,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    79,    80,    81,    82,    83,    84,
      85,    86,     0,    49,     0,    50,    67,     0,     0,     0,
      89,    90,    52,     0,     0,    53,    79,    80,    81,    82,
      83,    84,    85,    86
};

static const yytype_int8 yycheck[] =
{
      28,     3,     5,     5,     0,    11,     5,    35,    18,     8,
      16,    11,    22,    15,    10,    11,    16,    13,    46,    15,
      16,    17,    15,    19,    20,    53,    25,    23,    27,    28,
      29,    30,    10,    11,    30,    31,     3,    15,     5,     6,
       5,    14,    38,    16,    15,    12,    10,    11,    15,     5,
       6,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    10,    11,    15,    13,     0,    15,    16,
      17,     7,    19,    20,    13,    14,    23,    12,    15,     5,
       7,    17,    18,    19,    20,    21,    22,    23,    24,     7,
       5,    38,    16,     5,    11,    31,    32,    33,    34,    17,
      18,    19,    20,    21,    22,    23,    24,     7,    11,    13,
       8,     7,     7,    31,    32,    33,    34,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     7,     5,     7,     5,
       7,    -1,    31,    32,    33,    34,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     7,   104,     7,    35,    -1,
      -1,    31,    32,    33,    34,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    34,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,     3,    -1,     5,     6,    -1,    -1,    -1,
      33,    34,    12,    -1,    -1,    15,    17,    18,    19,    20,
      21,    22,    23,    24
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,     8,    25,    27,    28,    29,    30,    36,    37,
      40,    41,    42,    43,    46,    47,    49,    50,    51,    52,
      53,    54,    55,    56,    10,    11,    15,     5,    15,     5,
      15,    15,     0,    37,    37,    10,    11,    37,    12,    45,
      37,    37,    37,    45,    37,    37,    15,    45,    37,     3,
       5,     6,    12,    15,    39,    42,    44,     5,    16,     7,
      44,     5,     6,    42,    48,     5,    42,     6,    39,    44,
       5,    37,    44,     7,     8,    38,    44,     7,     7,    17,
      18,    19,    20,    21,    22,    23,    24,    31,    32,    33,
      34,     7,    16,    14,    16,    16,    16,     7,     7,     7,
      13,    16,     5,    13,    14,    16,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,     5,     7,
       7,     7,    38
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    35,    36,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    37,    38,    38,    39,    40,    41,    41,    41,
      42,    42,    43,    43,    43,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    45,    46,    47,    48,    48,    48,    48,    49,    50,
      50,    51,    52,    53,    54,    55,    56
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     3,     4,     4,     4,
       3,     3,     4,     4,     4,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     2,     1,     1,     1,     3,     5,     5,
       5,     2,     2,     4,     1,     4,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 220 "dtl.y" /* yacc.c:1646  */
    {
		if(DEBUG_MODE)
			cout << "FINISHED!" << endl;
	}
#line 1515 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 241 "dtl.y" /* yacc.c:1646  */
    {
			basicObject basicObjectCreated;
			basicObjectCreated.type = Integer;
			basicObjectCreated.obj = 0;

			(*auxForObjectInitialization)[(yyvsp[0].sval)] = basicObjectCreated;
			
			(yyval.oval) = auxForObjectInitialization;
		}
#line 1529 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 254 "dtl.y" /* yacc.c:1646  */
    {
		unordered_map<string, basicObject> *aux = (unordered_map<string, basicObject> *) (yyvsp[-1].oval);
		(yyval.oval) = aux;

		//Limpa o objeto no final, porque o objeto criado já foi passado para ser assimilado
		auxForObjectInitialization = new unordered_map<string, basicObject>();
		}
#line 1541 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 265 "dtl.y" /* yacc.c:1646  */
    {
		basicObject auxObject;
		auxObject.type = Integer;
		auxObject.obj = 0;
		string varName = (yyvsp[-1].sval);

		runProgram.push_back([auxObject, varName]() { 
			attr(cast(GLOBAL)[varName], auxObject); 

			if(DEBUG_MODE)
				cout << "VARIABLE DECLARATION: " << varName << endl;	
		});		
  }
#line 1559 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 281 "dtl.y" /* yacc.c:1646  */
    {
		string varName = (yyvsp[-3].sval);

		runProgram.push_back([varName]() { 
			basicObject auxObject;
			auxObject.type = Integer;
			auxObject.obj = globalExpressionStack.back();
			globalExpressionStack.pop_back();

			attr(cast(GLOBAL)[varName], auxObject); 

			if(DEBUG_MODE){
				cout << "VARIABLE ATTRIBUTION: (INTEGER) " << varName << " = ";
				decast(cast(GLOBAL)[varName]);	 	
			}
		});				
		
	}
#line 1582 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 299 "dtl.y" /* yacc.c:1646  */
    {
		
		basicObject auxObject;
		auxObject.type = String;
		string aux2 = (yyvsp[-1].sval);
		auxObject.obj = aux2;
		
		string varName = (yyvsp[-3].sval);

		runProgram.push_back([auxObject, varName]() { 
			attr(cast(GLOBAL)[varName], auxObject); 

			if(DEBUG_MODE){
				cout << "VARIABLE ATTRIBUTION: (STRING) " << varName << " = ";
				decast(cast(GLOBAL)[varName]);	 
			}	
		});				
	}
#line 1605 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 317 "dtl.y" /* yacc.c:1646  */
    {
		
		basicObject auxObject;
		auxObject.type = Object;
		auxObject.obj = (unordered_map<string, basicObject> *) (yyvsp[-1].oval);
		
		string varName = (yyvsp[-3].sval);

		runProgram.push_back([auxObject, varName]() { 
			attr(cast(GLOBAL)[varName], auxObject); 

			if(DEBUG_MODE){
				cout << "VARIABLE ATTRIBUTION: (OBJECT) " << varName << " = {" << endl;

				for(auto x : cast(cast(GLOBAL)[varName])){
					cout << "\t" << x.first << " = ";
					decast(x.second);
				}
				cout << "}" << endl;
			}
		});
	}
#line 1632 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 342 "dtl.y" /* yacc.c:1646  */
    { 
		auxForCascadedObjects.push_back((yyvsp[-2].sval));
		auxForCascadedObjects.push_back((yyvsp[0].sval));
	}
#line 1641 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 346 "dtl.y" /* yacc.c:1646  */
    {
		auxForCascadedObjects.push_back((yyvsp[0].sval));
	}
#line 1649 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 352 "dtl.y" /* yacc.c:1646  */
    {
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

			if(DEBUG_MODE){
				cout << "VARIABLE ATTRIBUTION: (INTEGER) ";
				
				for(auto x : localCascadedObjects) 
					if(x == localCascadedObjects.front())
						cout << x;
					else cout << "." << x;

				cout << " = ";

				decast(cast(auxDereference)[stringForPrintingLater]);
			}
		});

		auxForCascadedObjects.clear();		
	}
#line 1692 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 390 "dtl.y" /* yacc.c:1646  */
    {
		
		basicObject auxObject;
		auxObject.type = String;
		string aux2 = (yyvsp[-1].sval);
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

			if(DEBUG_MODE){
				cout << "VARIABLE ATTRIBUTION: (STRING) ";
				
				for(auto x : localCascadedObjects) 
					if(x == localCascadedObjects.front())
						cout << x;
					else cout << "." << x;

				cout << " = ";

				decast(cast(auxDereference)[stringForPrintingLater]);
			}
		});

		auxForCascadedObjects.clear();
	}
#line 1735 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 428 "dtl.y" /* yacc.c:1646  */
    {
		
		basicObject auxObject;
		auxObject.type = Object;
		auxObject.obj = (unordered_map<string, basicObject> *) (yyvsp[-1].oval);

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

			if(DEBUG_MODE){
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
			}

			//decast((cast(auxDereference))[stringForPrintingLater]);
		});

		auxForCascadedObjects.clear();		
	}
#line 1783 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 475 "dtl.y" /* yacc.c:1646  */
    {
		//$$ = $1;

			int numberFound = (yyvsp[0].ival);
			
			if(DEBUG_MODE)
				cout << "Program counter position: " << runProgram.size() << endl;

			runProgram.push_back([numberFound]() {
				if(DEBUG_MODE)
					cout << "Int pushed to expression stack " << globalExpressionStack.size() << endl;

				globalExpressionStack.push_back(numberFound);

				if(shouldDuplicateExpresssionStack)
					globalExpressionStackDuplicate.push_back(numberFound);
			});
			
		}
#line 1807 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 494 "dtl.y" /* yacc.c:1646  */
    {
			string varName = (yyvsp[0].sval);

			runProgram.push_back([varName]() {
				if(DEBUG_MODE)
					cout << "Var pushed to expression stack " << globalExpressionStack.size() << endl;

				globalExpressionStack.push_back(((cast(GLOBAL)[varName]).obj));

				if(shouldDuplicateExpresssionStack)
					globalExpressionStackDuplicate.push_back(varName);
			});
		}
#line 1825 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 507 "dtl.y" /* yacc.c:1646  */
    {
		vector<string> localCascadedObjects = auxForCascadedObjects;

		runProgram.push_back([localCascadedObjects]() { 			
			string stringForPrintingLater;
			basicObject auxDereference = GLOBAL;
			for(auto x : localCascadedObjects){
				if(x == localCascadedObjects.back()){ //Se for o ultimo elemento rola a atribuição nele
					globalExpressionStack.push_back(cast(auxDereference)[x].obj);
					
					stringForPrintingLater = x;				
				} else { //Se não vai descendo recursivamente
					auxDereference = cast(auxDereference)[x];
				}
			}

			if(DEBUG_MODE){
				cout << "VARIABLE ATTRIBUTION: (INTEGER) ";
				
				for(auto x : localCascadedObjects) 
					if(x == localCascadedObjects.front())
						cout << x;
					else cout << "." << x;

				cout << " = ";

				decast(cast(auxDereference)[stringForPrintingLater]);
			}
		});

		auxForCascadedObjects.clear();
	}
#line 1862 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 539 "dtl.y" /* yacc.c:1646  */
    {
				//$$ = $1 / $3;
				
				runProgram.push_back([](){
					doOperationWithExpression(3);
				}); 

				if(shouldDuplicateExpresssionStack)
					globalOperationStackDuplicate.push_back([](){
						doOperationWithExpression(3);
					}); 
			}
#line 1879 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 551 "dtl.y" /* yacc.c:1646  */
    {
			//$$ = $1 * $3;
			runProgram.push_back([](){
					doOperationWithExpression(2);
			}); 
		}
#line 1890 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 557 "dtl.y" /* yacc.c:1646  */
    {
			//$$ = $1 + $3;
			runProgram.push_back([](){
				doOperationWithExpression(0);
			}); 	
		}
#line 1901 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 563 "dtl.y" /* yacc.c:1646  */
    {
		//$$ = $1 - $3;
		runProgram.push_back([](){
				doOperationWithExpression(1);
		}); 	
	}
#line 1912 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 569 "dtl.y" /* yacc.c:1646  */
    {
		runProgram.push_back([](){
				doOperationWithExpression(4);
		}); 
	}
#line 1922 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 574 "dtl.y" /* yacc.c:1646  */
    {
		runProgram.push_back([](){
				doOperationWithExpression(5);
		}); 
	}
#line 1932 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 579 "dtl.y" /* yacc.c:1646  */
    {
		runProgram.push_back([](){
				doOperationWithExpression(6);
		}); 
	}
#line 1942 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 584 "dtl.y" /* yacc.c:1646  */
    {
		runProgram.push_back([](){
				doOperationWithExpression(7);
		}); 
	}
#line 1952 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 589 "dtl.y" /* yacc.c:1646  */
    {
		runProgram.push_back([](){
				doOperationWithExpression(8);
		}); 
	}
#line 1962 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 594 "dtl.y" /* yacc.c:1646  */
    {
		runProgram.push_back([](){
				doOperationWithExpression(9);
		}); 
	}
#line 1972 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 599 "dtl.y" /* yacc.c:1646  */
    {
		runProgram.push_back([](){
				doOperationWithExpression(10);
		}); 
	}
#line 1982 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 604 "dtl.y" /* yacc.c:1646  */
    {
		runProgram.push_back([](){
				doOperationWithExpression(11);
		}); 
	}
#line 1992 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 609 "dtl.y" /* yacc.c:1646  */
    {
		// Tá certo, tem que parsear os internos primeiros
		// então fica essa declaração para dar prioridade na hora de jogar
		// nas respectivas stacks
	}
#line 2002 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 620 "dtl.y" /* yacc.c:1646  */
    { //Reconhece esse padrão quando começa o if
		ifHeadStack.push_back(runProgram.size());
	}
#line 2010 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 625 "dtl.y" /* yacc.c:1646  */
    { 
		//Reconhece esse padrão quando acaba o if
		ifFootStack.push_back(runProgram.size()); 

		if(DEBUG_MODE){
			cout << "IF HEAD STACK: ";
			for(auto x : ifHeadStack)
				cout << x << " ";
			cout << endl;

			cout << "IF FOOT STACK: ";
			for(auto x : ifFootStack)
				cout << x << " ";
			cout << endl;

			cout << "FECHANDO UM IF:";
		}

		int ifHead = ifHeadStack.back();
		int ifFoot = ifFootStack.back();

		ifHeadStack.pop_back();
		ifFootStack.pop_back();

		if(DEBUG_MODE){
			cout << ifHead << " " << ifFoot << endl;
		}

		// AQUI EU COLOCO UMA FUNÇÃO NO RUNPROGRAM QUE FAZ O CHEQUE DOS IFS, ESSA FUNCAO FICA 
		// NA POSICAO IFHEAD (INSERT), COMPARA O STATEMENT E CONTINUA OU PULA A EXECUÇÃO PRO
		// IFFOOT
		
		//runProgram checkIf(ifHead, ifFoot, $1); falta dar o Insert e jogar no runprogram
		vector<function<void()>>::iterator ifHeadPositionIterator = runProgram.begin() + ifHead;
		runProgram.insert(ifHeadPositionIterator, [ifHead, ifFoot]() { 
			int conditionResult = any_cast<int>(globalExpressionStack.back()); //O resultado da expressão está no final do Expression Stack
			globalExpressionStack.pop_back();

			checkIf(ifHead, ifFoot, conditionResult); 
		});	

	}
#line 2057 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 671 "dtl.y" /* yacc.c:1646  */
    {
		string varName = (yyvsp[0].sval);
		runProgram.push_back([varName]() {
			cout << varName << " = ";
			decast(cast(GLOBAL)[varName]);
		});}
#line 2068 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 677 "dtl.y" /* yacc.c:1646  */
    {
		string textToPrint = (yyvsp[0].sval);
		runProgram.push_back([textToPrint]() {
			//retirando as aspas
			cout << textToPrint.substr(1, textToPrint.length() - 2) << endl;
		});}
#line 2079 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 683 "dtl.y" /* yacc.c:1646  */
    {
			vector<string> localCascadedObjects = auxForCascadedObjects;

			runProgram.push_back([localCascadedObjects]() { 			
				string stringForPrintingLater;
				basicObject auxDereference = GLOBAL;
				for(auto x : localCascadedObjects){
					if(x == localCascadedObjects.back()){ //Se for o ultimo elemento rola a atribuição nele						
						stringForPrintingLater = x;				
					} else { //Se não vai descendo recursivamente
						auxDereference = cast(auxDereference)[x];
					}
				}
								
				for(auto x : localCascadedObjects) 
					if(x == localCascadedObjects.front())
						cout << x;
					else cout << "." << x;

				cout << " = ";

				decast(cast(auxDereference)[stringForPrintingLater]);
			});
		}
#line 2108 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 707 "dtl.y" /* yacc.c:1646  */
    {
		string varName = (yyvsp[0].sval);
		runProgram.push_back([varName]() {
			cout << varName << " = ";
			decast(cast(GLOBAL)[varName]);
		});}
#line 2119 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 721 "dtl.y" /* yacc.c:1646  */
    {
		string varName = (yyvsp[-2].sval);

		runProgram.push_back([varName]() { 
			int intFromCin = 0;
			basicObject auxObject;
			auxObject.type = Integer;
			cin >> intFromCin;
			auxObject.obj = intFromCin;

			attr(cast(GLOBAL)[varName], auxObject); 

			if(DEBUG_MODE){
				cout << "VARIABLE SCAN: (INTEGER) " << varName << " = ";
				decast(cast(GLOBAL)[varName]);	 	
			}
		});		
	}
#line 2142 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 739 "dtl.y" /* yacc.c:1646  */
    {
		vector<string> localCascadedObjects = auxForCascadedObjects;

		runProgram.push_back([localCascadedObjects]() {
			int intFromCin; 			
			basicObject auxObject;
			auxObject.type = Integer;
			cin >> intFromCin;
			auxObject.obj = intFromCin;

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

			if(DEBUG_MODE){
				cout << "VARIABLE ATTRIBUTION: (INTEGER) ";
				
				for(auto x : localCascadedObjects) 
					if(x == localCascadedObjects.front())
						cout << x;
					else cout << "." << x;

				cout << " = ";

				decast(cast(auxDereference)[stringForPrintingLater]);
			}
		});

		auxForCascadedObjects.clear();
	}
#line 2185 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 780 "dtl.y" /* yacc.c:1646  */
    { //Reconhece esse padrão quando começa a declaração de função
		functionMap[(yyvsp[0].sval)] = runProgram.size();
		functionHeadStack.push_back(runProgram.size());		
	}
#line 2194 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 786 "dtl.y" /* yacc.c:1646  */
    { 
		//Reconhece esse padrão quando acaba a declaração de função
		functionFootStack.push_back(runProgram.size()); 

		if(DEBUG_MODE){
			cout << "FUNCTION HEAD STACK: ";
			for(auto x : functionHeadStack)
				cout << x << " ";
			cout << endl;

			cout << "FUNCTION FOOT STACK: ";
			for(auto x : functionFootStack)
				cout << x << " ";
			cout << endl;

			cout << "FECHANDO UMA FUNCAO:";
		}

		int functionHead = functionHeadStack.back();
		int functionFoot = functionFootStack.back();

		functionHeadStack.pop_back();
		functionFootStack.pop_back();

		// Aqui eu pulo a declaração do corpo da função de executar e insiro no seu pé um return		
		vector<function<void()>>::iterator functionHeadPositionIterator = runProgram.begin() + functionHead;
		runProgram.insert(functionHeadPositionIterator, [functionHead, functionFoot]() { 
			globalProgramIterator += functionFoot - functionHead + 1; //+1 para considerar o retorno da função também
		});

		vector<function<void()>>::iterator functionFootPositionIterator = runProgram.begin() + functionFoot;
		runProgram.push_back([]() { 
			if(DEBUG_MODE)
				cout << "RETURNING FROM FUNCTION" << endl;			

			//vector<function<void()>>::iterator returnIterator = ;
			globalProgramIterator = functionReturnStack.back();
			functionReturnStack.pop_back();
		});
	}
#line 2239 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 828 "dtl.y" /* yacc.c:1646  */
    {
		string functionName = (yyvsp[-3].sval);

		runProgram.push_back([functionName]() { 
			functionReturnStack.push_back(globalProgramIterator);

			if(DEBUG_MODE){
				cout << "CALLING FUNCTION " << functionName << " ON ADDRESS " << functionMap[functionName] << endl;
				cout << "RETURN ADDRESS SAVED" << endl;
			}

			//vector<function<void()>>::iterator returnIterator = ;
			globalProgramIterator = runProgram.begin() + functionMap[functionName];
		});
	}
#line 2259 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 846 "dtl.y" /* yacc.c:1646  */
    {
		if(DEBUG_MODE)
			cout << "ATOMIC WHILE FOUND: " << runProgram.size() << endl; 
		shouldDuplicateExpresssionStack = true;

		runProgram.push_back([]() {});
		whileHeadStack.push_back(runProgram.size());
	}
#line 2272 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 856 "dtl.y" /* yacc.c:1646  */
    { //Reconhece esse padrão quando começa o if
		whileBodyStack.push_back(runProgram.size());
		shouldDuplicateExpresssionStack = false;
	}
#line 2281 "dtl.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 862 "dtl.y" /* yacc.c:1646  */
    { 
		//Reconhece esse padrão quando acaba o if
		whileFootStack.push_back(runProgram.size()); 

		if(DEBUG_MODE){
			cout << "WHILE HEAD STACK: ";
			for(auto x : whileHeadStack)
				cout << x << " ";
			cout << endl;

			cout << "WHILE FOOT STACK: ";
			for(auto x : whileFootStack)
				cout << x << " ";
			cout << endl;

			cout << "FECHANDO UM WHILE:";
		}

		int whileHead = whileHeadStack.back();
		int whileFoot = whileFootStack.back();
		int whileBody = whileBodyStack.back();

		whileHeadStack.pop_back();
		whileFootStack.pop_back();
		whileBodyStack.pop_back();

		if(DEBUG_MODE){
			cout << whileHead << " " << whileFoot << endl;
		}
		
		vector<function<void()>>::iterator whileBodyPositionIterator = runProgram.begin() + whileBody;
		runProgram.insert(whileBodyPositionIterator, [whileHead, whileFoot]() { 
			int conditionResult = any_cast<int>(globalExpressionStack.back()); //O resultado da expressão está no final do Expression Stack
			globalExpressionStack.pop_back();

			checkIf(whileHead, whileFoot - 2, conditionResult); 
		});	

		runProgram.push_back([whileHead]() { 
			if(DEBUG_MODE)
				cout << " GO TO HEAD OF WHILE " << whileHead - 1 << endl;			

			globalProgramIterator = runProgram.begin() + whileHead - 1;
		});
	}
#line 2331 "dtl.tab.c" /* yacc.c:1646  */
    break;


#line 2335 "dtl.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 909 "dtl.y" /* yacc.c:1906  */


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

		if(DEBUG_MODE){
			for(auto i : globalExpressionStack)
				if(anyTypeString.compare(i.type().name()) == 0)
					cout << any_cast<string>(i) << ", ";
				else
					cout << any_cast<int>(i) << ", ";
			cout << endl;
		}
	}
}

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}
