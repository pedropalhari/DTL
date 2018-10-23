/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
#line 174 "dtl.y" /* yacc.c:1909  */

	int ival;
	float fval;
	char *sval;
	void *oval; //Object

#line 96 "dtl.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_DTL_TAB_H_INCLUDED  */
