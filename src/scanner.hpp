#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif //yyFlexLexer

#include "variable_map.hpp"

namespace calc {

class Scanner : public yyFlexLexer {
    VariableMap& var_map;
public:
    Scanner(std::istream& arg_yyin, std::ostream& arg_yyout, VariableMap& var_map)
        : yyFlexLexer(arg_yyin, arg_yyout), var_map(var_map) {}
    Parser::symbol_type lex();
};

}
