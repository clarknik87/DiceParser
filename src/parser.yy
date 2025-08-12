%{
#include <iostream>
#include <string>
#include <cmath>
#include <FlexLexer.h>
%}

%require "3.7.4"
%language "C++"
%defines "parser.hpp"

%code requires
{
    #include "parser_result.hpp"
}

%define api.parser.class {Parser}
%define api.namespace {calc}
%define api.value.type variant
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

%token <double>  DOUBLE_T

%nterm <double>  dexp

%token PLUS
%token MINUS
%token MULTIPLY
%token DIVIDE

%left               PLUS MINUS
%left               MULTIPLY DIVIDE
%precedence         UMINUS

%%

input:
  dexp                { result = parse_result_t{$1}; }
  ;

dexp:
  DOUBLE_T            { $$ = $1;}
| dexp PLUS     dexp  { $$ = $1 + $3; }
| dexp MINUS    dexp  { $$ = $1 - $3; }
| dexp MULTIPLY dexp  { $$ = $1 * $3; }
| dexp DIVIDE   dexp  { $$ = $1 / $3; }

%%

void calc::Parser::error(const std::string& msg) {
    std::cerr << msg << '\n';
}