#ifndef SCANNER_HPP_GUARD
#define SCANNER_HPP_GUARD

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif //yyFlexLexer
#include "parser.hpp"
#include "variable_map.hpp"
#include <string>
#include <sstream>

namespace calc {

class Scanner : public yyFlexLexer {
    VariableMap& var_map;
    std::string full_input;
    std::ostringstream errstream;
    std::istringstream istream;
public:
    Scanner(std::istream& arg_yyin, std::ostream& arg_yyout, VariableMap& var_map)
        : yyFlexLexer(arg_yyin, arg_yyout), var_map(var_map) {}

    Parser::symbol_type lex();

    void set_input(const std::string& input)
    {
        full_input = input;
        errstream.clear();
        istream.clear();
        istream.str(input);
        switch_streams(istream, errstream);
    }

    std::string get_full_input() const
    {
        return full_input;
    }

    std::string get_assigned_expr() const
    {
        std::size_t idx = 0;
        if(full_input.rfind("=") != full_input.npos)
            idx = full_input.rfind("=")+1;
        return full_input.substr(idx);
    }
};

}

#endif //SCANNER_HPP_GUARD
