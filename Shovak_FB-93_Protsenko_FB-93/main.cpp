#include "parser.h"
#include "iostream"

int main(){
    std::string input = "SELECT owner_name, cat_name \n  FROM owners\n  LEFT_JOIN cats ON owner_id = cat_owner_id;";
    Parser parser(input);
    parser.getNextCommand();
    return 0;
}