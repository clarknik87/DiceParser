%{
#include <iostream>
#include <string>
#include <cmath>
#include <FlexLexer.h>
%}

%require "3.7.4"
%language "C++"
%skeleton "lalr1.cc"
%defines "parser.hpp"

%code requires
{
    #include "dice_parser/parser_result.hpp"
}

%define api.parser.class {Parser}
%define api.namespace {calc}
%define api.token.constructor
%define api.value.type variant
%define api.token.raw
%parse-param {Scanner* scanner} {parse_result_t& result}

%code requires
{
    namespace calc {
        class Scanner;
    } // namespace calc
} // %code requires

%code
{
    #include "dice_distribution.hpp"
    #include "scanner.hpp"
    #define yylex(x) scanner->lex(x)
}

%token <double>               DOUBLE_T
%token <DiceDistr>            DICE_T

%nterm <double>               expr
%nterm <DiceDistr>            dexpr

%token                        PLUS
%token                        MINUS
%token                        MULTIPLY
%token                        DIVIDE
%token                        LPAREN
%token                        RPAREN
%token                        EQUAL_TO
%token                        NOT_EQUAL_TO
%token                        GREATER_EQUA
%token                        LESS_EQUAL
%token                        GREATER_THAN
%token                        LESS_THAN
%token                        STATS

%left                         PLUS MINUS
%left                         MULTIPLY DIVIDE
%precedence                   UMINUS

%%

input:
  expr                        { result = parse_result_t{$1}; }
  | dexpr                     { result = parse_result_t{$1.roll()}; }
  | STATS LPAREN dexpr RPAREN { result = parse_result_t{$3}; }
  | dexpr EQUAL_TO expr       { result = parse_result_t{$1 == $3}; }
  | dexpr NOT_EQUAL_TO expr   { result = parse_result_t{$1 != $3}; }
  | dexpr GREATER_EQUA expr   { result = parse_result_t{$1 >= $3}; }
  | dexpr LESS_EQUAL expr     { result = parse_result_t{$1 <= $3}; }
  | dexpr GREATER_THAN expr   { result = parse_result_t{$1 > $3}; }
  | dexpr LESS_THAN expr      { result = parse_result_t{$1 < $3}; }
  | expr EQUAL_TO dexpr       { result = parse_result_t{$1 == $3}; }
  | expr NOT_EQUAL_TO dexpr   { result = parse_result_t{$1 != $3}; }
  | expr GREATER_EQUA dexpr   { result = parse_result_t{$1 >= $3}; }
  | expr LESS_EQUAL dexpr     { result = parse_result_t{$1 <= $3}; }
  | expr GREATER_THAN dexpr   { result = parse_result_t{$1 > $3}; }
  | expr LESS_THAN dexpr      { result = parse_result_t{$1 < $3}; }
  ;

expr:
  DOUBLE_T                    { $$ = $1; }
| PLUS expr %prec UMINUS      { $$ = +$2; }
| MINUS expr %prec UMINUS     { $$ = -$2; }
| expr PLUS     expr          { $$ = $1 + $3; }
| expr MINUS    expr          { $$ = $1 - $3; }
| expr MULTIPLY expr          { $$ = $1 * $3; }
| expr DIVIDE   expr          { $$ = $1 / $3; }
| LPAREN expr RPAREN          { $$ = $2; }

dexpr:
  DICE_T                      { $$ = $1; }
| PLUS dexpr %prec UMINUS     { $$ = +$2; }
| MINUS dexpr %prec UMINUS    { $$ = -$2; }
| dexpr PLUS     dexpr        { $$ = $1 + $3; }
| dexpr MINUS    dexpr        { $$ = $1 - $3; }
| dexpr PLUS     expr         { $$ = $1 + $3; }
| dexpr MINUS    expr         { $$ = $1 - $3; }
| dexpr MULTIPLY expr         { $$ = $1 * $3; }
| expr PLUS     dexpr         { $$ = $1 + $3; }
| expr MINUS    dexpr         { $$ = $1 - $3; }
| expr MULTIPLY dexpr         { $$ = $1 * $3; }
| LPAREN dexpr RPAREN         { $$ = $2; }

%%

void calc::Parser::error(const std::string& msg) {
    throw err_code::invalid_syntax;
}