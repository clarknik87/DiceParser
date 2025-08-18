#include <string>
#include "dice_parser/dice_parser.hpp"


int main(int argc, char* argv[])
{
    std::cout << "****************************************************************" << std::endl;
    std::cout << "*   ___  _          ___                       ___          _   *" << std::endl;
    std::cout << "*  |   \\(_)__ ___  | _ \\__ _ _ _ ______ _ _  | _ \\___ _ __| |  *" << std::endl;
    std::cout << "*  | |) | / _/ -_) |  _/ _` | '_(_-/ -_| '_| |   / -_| '_ | |  *" << std::endl;
    std::cout << "*  |___/|_\\__\\___| |_| \\__,_|_| /__\\___|_|   |_|_\\___| .__|_|  *" << std::endl;
    std::cout << "*                                                    |_|       *" << std::endl;
    std::cout << "*  enter  \"quit \" to exit program                              *" << std::endl;
    std::cout << "****************************************************************" << std::endl;

    DiceParser parser;
    std::string user_input;
    while(user_input != "quit")
    {
        std::cout << ">> ";
        std::getline(std::cin, user_input);
        if(user_input != "quit" && (user_input.find_first_not_of(" \t\n\v\f\r") != std::string::npos))
        {
            auto result = parser.parse(user_input);
            if(std::holds_alternative<double>(result))
                std::cout << ">> " << std::get<double>(result) <<std::endl;
            else if(std::holds_alternative<DiceDistr>(result))
                std::cout << ">> " << std::get<DiceDistr>(result).get_expr() <<std::endl;
            else if(std::holds_alternative<action_code>(result))
                std::cout << ">> " << std::get<action_code>(result) <<std::endl;
            else
                std::cout << ">> " << "unknown return type" << std::endl;
        }
    }
}
