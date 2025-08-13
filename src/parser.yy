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
    #include "parser_result.hpp"
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

%token <double>             DOUBLE_T
%token <DiceDistr>          DICE_T

%nterm <double>             expr
%nterm <DiceDistr>          dexpr

%token                      PLUS
%token                      MINUS
%token                      MULTIPLY
%token                      DIVIDE
%token                      LPAREN
%token                      RPAREN

%left                       PLUS MINUS
%left                       MULTIPLY DIVIDE
%precedence                 UMINUS

%%

input:
  expr                      { result = parse_result_t{$1}; }
  | dexpr                   { result = parse_result_t{$1.roll()}; }
  ;

expr:
  DOUBLE_T                  { $$ = $1; }
| PLUS expr %prec UMINUS    { $$ = +$2; }
| MINUS expr %prec UMINUS   { $$ = -$2; }
| expr PLUS     expr        { $$ = $1 + $3; }
| expr MINUS    expr        { $$ = $1 - $3; }
| expr MULTIPLY expr        { $$ = $1 * $3; }
| expr DIVIDE   expr        { $$ = $1 / $3; }
| LPAREN expr RPAREN        { $$ = $2; }

dexpr:
  DICE_T                    { $$ = $1; }

%%

void calc::Parser::error(const std::string& msg) {
    std::cerr << msg << '\n';
}