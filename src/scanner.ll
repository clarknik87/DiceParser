/* Scanner for DiceParse
 */

%{
#include "parser.hpp"
#include "scanner.hpp"
#include "dice_distribution.hpp"
#define YY_DECL int calc::Scanner::lex(calc::Parser::semantic_type *yylval)
// workaround for bug in flex 2.6.4
#define yypanic(X) (void)(X)
%}

%option c++
%option noyywrap
%option noyylineno
%option nodefault

int_t     [0-9]+
double_t  [0-9]+\.[0-9]+

%%
{int_t}           {yylval->emplace<double>(strtod(YYText(), nullptr)); return Parser::token::DOUBLE_T;}
{double_t}        {yylval->emplace<double>(strtod(YYText(), nullptr)); return Parser::token::DOUBLE_T;}
"+"               return Parser::token::PLUS;
"-"               return Parser::token::MINUS;
"*"               return Parser::token::MULTIPLY;
"/"               return Parser::token::DIVIDE;
[ \t\r\n]+        { /* skip whitespace */ }
.                 { return Parser::token::YYerror; }

%%



int yyFlexLexer::yylex() {
    throw std::runtime_error("Invalid call to yyFlexLexer::yylex()");
}
