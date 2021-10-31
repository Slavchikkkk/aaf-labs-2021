#include "parser.h"
#include "iostream"

int main(){
    std::string input = "INSERT INTO owners(\"1\", \"Vasya\", \"30\");.";

    Parser parser(input);
    parser.getNextCommand();
    return 0;
}