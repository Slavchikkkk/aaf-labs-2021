#include "db.h"
#include <iostream>

std::vector<std::string> deleteFirst(std::vector<std::string>& arr)
{
    auto start = arr.begin() + 1;
 
    std::vector<std::string> result(arr.size()-1);
 
    copy(start, arr.end(), result.begin());
 
    return result;
}

void db::executeCommand(std::string input) 
{
    Parser parser(input);
    std::vector<std::string> arguments = parser.getNextCommand();
    std::string command = arguments[0];
    arguments = deleteFirst(arguments);

    bool status = false;
    if (command == "CREATE") {
        status = createTable(arguments);
    }  else  if (command == "INSERT") {
        status = insertInTable(arguments);
    } else if (command == "DELETE") {
        status = deleteInTable(arguments);
    }

    if (!status) {
        std::cout << std::endl << command << " - is wrong command";
    }
}

int db::getTableIndex(const std::string& name) 
{
    for (int i = 0; i < m_tables.size(); i++) {
        if (m_tables[i].getName() == name) 
            return i;      
    }
    return -1;
}

bool db::createTable(std::vector<std::string>& parameters) 
{
    int pos = getTableIndex(parameters[0]);
    if (pos != -1){
        //error
        std::cout << "Table " << parameters[0] << " already exists.\n";
        return false;
    }
    pos = m_tables.size();
    m_tables.push_back(Table(parameters[0]));

    parameters = deleteFirst(parameters);
    std::vector<std::string> arguments;
    for (std::string i : parameters) {
        if (i == "INDEXED") continue;
        arguments.push_back(i);
    }
    
    m_tables[pos].setCollumnNames(arguments);
    return true;
}

bool db::insertInTable(std::vector<std::string>& parameters) 
{
    int pos = getTableIndex(parameters[0]);
    if (pos == -1) {
        //error
        std::cout << "Table " << parameters[0] << " does not exist." << std::endl;
        return false;
    }

    parameters = deleteFirst(parameters);
    if (m_tables[pos].getCollumnCount() != parameters.size()) {
        std::cout << "Not consistent amount of parameters " << parameters.size() << std::endl;
        return false;
    }
    m_tables[pos].insertRow(parameters);
    return true;
}

bool db::deleteInTable(std::vector<std::string>& parameters) 
{
    int pos = getTableIndex(parameters[0]);
    if (pos == -1) {
        //error
        return false;
    }
    parameters = deleteFirst(parameters);
    if (parameters.size() >= 1 && parameters[0] == "WHERE") {
        parameters = deleteFirst(parameters);
        m_tables[pos].deleteWithCondition(parameters);
    } else {
        m_tables[pos].deleteAllRows();
    }

    return true;
}

void db::printTables() 
{
    for (auto i : m_tables){
        i.printTable();
    }
}

Table::Table() 
    : m_collumnCount(0)
    , m_collumnNames()
    , m_name("")
    , m_rows()
{}

Table::Table(const std::string& name) 
    : m_collumnCount(0)
    , m_collumnNames()
    , m_name(name)
    , m_rows()
{}


std::string Table::getName() 
{
    return m_name;
}

int Table::getCollumnCount() 
{
    return m_collumnCount;
}

void Table::insertRow(const std::vector<std::string>& row) 
{
    m_rows.push_back(row);
}

void Table::setCollumnNames(const std::vector<std::string>& names) 
{
    m_collumnNames = names;
    m_collumnCount = names.size();
}

void Table::deleteAllRows() 
{
    m_rows.clear();
}

int Table::getCollumnNameIndex(const std::string& name) 
{
    for (int i = 0; i < m_collumnCount; i++) {
        if (m_collumnNames[i] == name) return i;
    }
    return -1;
}

void Table::deleteWithCondition(const std::vector<std::string>& condition) 
{
    int pos = getCollumnNameIndex(condition[0]);
    if (pos == -1) {
        //error
    }
    if (condition[1] == "=") {
        for (int i = 0; i < m_rows.size(); i++){
            if (m_rows[i][pos] == condition[2]) {
                m_rows.erase(m_rows.begin() + i);
            }
        }
    } else if (condition[1] == "!=") {
        for (int i = 0; i < m_rows.size(); i++){
            if (m_rows[i][pos] != condition[2]) {
                m_rows.erase(m_rows.begin() + i);
            }
        }
    }
}

void printHorizontalLane(int count) {
    for (int i = 0; i <= count; i++){
        std::cout << "_";
    }
    std::cout << std::endl;
}

void Table::printTable() 
{
    std::cout << "\t" << m_name << "\t" << std::endl;
    printHorizontalLane(m_collumnCount*15);
    for (auto i : m_collumnNames){
        std::cout << i << '\t' << '|';
    }
    std::cout << std::endl;
    printHorizontalLane(m_collumnCount*15);
    for (auto row: m_rows){
        for (auto i : row){
            std::cout << i << '\t' << '|';
        }
        std::cout << std::endl;
        printHorizontalLane(m_collumnCount*15);
    }
}



db::db()
    : m_tables()
{
    
}
