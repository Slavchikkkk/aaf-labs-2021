#include "db.h"
#include "iostream"

int main(){
    std::string input = "CREATE owners (owner_id INDEXED, owner_name, owner_age);";

    db database;
    database.executeCommand(input);
    std::cout << std::endl;

    input = "INSERT INTO owners (\"1\", \"pedro\", \"20\");";
    database.executeCommand(input);
    std::cout << std::endl;

    database.printTables();

    input = "DELETE owners WHERE owner_id = \"1\";";
    database.executeCommand(input);
    std::cout << std::endl;

    database.printTables();
    return 0;
}
