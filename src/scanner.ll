/* Scanner for DiceParse
 */

%{
#include "parser.hpp"
#include "scanner.hpp"
#include "dice_distribution.hpp"
#define YY_DECL calc::Parser::symbol_type calc::Scanner::lex()
// workaround for bug in flex 2.6.4
#define yypanic(X) (void)(X)
%}

%option c++
%option noyywrap
%option noyylineno
%option nodefault

int_t     [0-9]+
double_t  [0-9]+\.[0-9]+
dice_t    [0-9]+d[0-9]+

%%
{dice_t}          return calc::Parser::make_DICE_T(DiceDistr(YYText()));
{int_t}           return calc::Parser::make_DOUBLE_T(strtod(YYText(), nullptr));
{double_t}        return calc::Parser::make_DOUBLE_T(strtod(YYText(), nullptr));
"+"               return calc::Parser::make_PLUS();
"-"               return calc::Parser::make_MINUS();
"*"               return calc::Parser::make_MULTIPLY();
"/"               return calc::Parser::make_DIVIDE();
"("               return calc::Parser::make_LPAREN();
")"               return calc::Parser::make_RPAREN();
[ \t\r\n]+        { /* skip whitespace */ }
.                 return calc::Parser::make_YYerror();

%%

int yyFlexLexer::yylex() {
    throw std::runtime_error("Invalid call to yyFlexLexer::yylex()");
}
