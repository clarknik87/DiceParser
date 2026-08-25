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
    #include "variable_map.hpp"
}

%define api.parser.class {Parser}
%define api.namespace {calc}
%define api.token.constructor
%define api.value.type variant
%define api.token.raw
%parse-param {Scanner* scanner} {parse_result_t& result} {VariableMap& var_map}

%code requires
{
    namespace calc {
        class Scanner;
    } // namespace calc
} // %code requires

%code
{
    #include "builtin_func.hpp"
    #include "dice_distribution.hpp"
    #include "scanner.hpp"
    #define yylex(x) scanner->lex(x)
}

%token <DiceDistr>            DICE_T
%token <std::string>          DICE_VARIABLE
%token <std::string>          NEW_VARIABLE
%token <std::string>          FUNC_ONE_ARG
%token <std::string>          FUNC_TWO_ARG

%nterm <DiceDistr>            dexpr

%token                        PLUS
%token                        MINUS
%token                        MULTIPLY
%token                        DIVIDE
%token                        LPAREN
%token                        RPAREN
%token                        EQUAL_TO
%token                        NOT_EQUAL_TO
%token                        GREATER_EQUAL
%token                        LESS_EQUAL
%token                        GREATER_THAN
%token                        LESS_THAN
%token                        ASSIGN
%token                        COMMA
%token                        OP_DELETE


%left                         PLUS MINUS
%left                         MULTIPLY DIVIDE
%left                         EQUAL_TO NOT_EQUAL_TO GREATER_EQUAL LESS_EQUAL GREATER_THAN LESS_THAN
%precedence                   UMINUS

%%

input:
    %empty                    { result = parse_result_t{action_code::empty_command};}  
  |dexpr                      { if($1.is_scalar()) result = parse_result_t{$1.get_scalar()}; else result = parse_result_t{$1};}
  | NEW_VARIABLE ASSIGN dexpr { var_map.add_variable($1, $3, scanner->get_assigned_expr()); result = parse_result_t{action_code::action_success}; }
  | DICE_VARIABLE ASSIGN dexpr{ var_map.add_variable($1, $3, scanner->get_assigned_expr()); result = parse_result_t{action_code::action_success}; }
  | OP_DELETE LPAREN DICE_VARIABLE RPAREN { var_map.delete_variable($3); result = parse_result_t{action_code::action_success}; }
  ;

dexpr:
  DICE_T                      { $$ = $1; }
| DICE_VARIABLE               { $$ = var_map.at($1); }
| NEW_VARIABLE                { throw action_code::variable_undefined; }
| PLUS dexpr %prec UMINUS     { $$ = +$2; }
| MINUS dexpr %prec UMINUS    { $$ = -$2; }
| dexpr PLUS     dexpr        { $$ = $1 + $3; }
| dexpr MINUS    dexpr        { $$ = $1 - $3; }
| dexpr MULTIPLY dexpr        { $$ = $1 * $3; }
| dexpr DIVIDE   dexpr        { $$ = $1 / $3; }
| dexpr EQUAL_TO dexpr        { $$ = $1 == $3; }
| dexpr NOT_EQUAL_TO dexpr    { $$ = $1 != $3; }
| dexpr GREATER_EQUAL dexpr    { $$ = $1 >= $3; }
| dexpr LESS_EQUAL dexpr      { $$ = $1 <= $3; }
| dexpr GREATER_THAN dexpr    { $$ = $1 > $3; }
| dexpr LESS_THAN dexpr       { $$ = $1 < $3; }
| LPAREN dexpr RPAREN         { $$ = $2; }
| FUNC_ONE_ARG LPAREN dexpr RPAREN { $$ = builtin::call_one_arg_func($1,$3); }
| FUNC_TWO_ARG LPAREN dexpr COMMA dexpr RPAREN { $$ = builtin::call_two_arg_func($1,$3, $5); }

%%

void calc::Parser::error(const std::string& msg) {
    throw action_code::invalid_syntax;
}