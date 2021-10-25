#include "parser.h"
#include "iostream"

int main(){
    std::string input = "DELETE cats WHERE name = \"Murzik\";";

    Parser parser(input);
    parser.getNextCommand();
    return 0;
}