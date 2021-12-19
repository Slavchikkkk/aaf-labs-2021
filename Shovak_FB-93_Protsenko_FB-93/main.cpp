#include "db.h"
#include "iostream"
#include <string>

int main(){
    db database{};
    char c;
    std::string input;
    while (true) {
        std::cout << "Enter command (type EXIT; to end programm): " << std::endl;
        do {
        c = getchar();
        input.push_back(c);
        } while (c != ';');
        if (input.find("EXIT;") != std::string::npos) {
            break;
        }
        database.executeCommand(input);
        input.clear();
    }

    return 0;
}