#include "db.h"
#include <iostream>
#include <algorithm>

void printHorizontalLane(int count) {
    for (int i = 0; i <= count; i++){
        std::cout << "_";
    }
    std::cout << std::endl;
}

void db::executeCommand(std::string input) 
{
    Parser parser(input);
    std::vector<std::string> arguments = parser.getNextCommand();
    if (arguments.empty()){
        std::cout << "Please check input" << std::endl;
        return;
    }
    std::string command = arguments[0];
    arguments.erase(arguments.begin());

    bool status = false;
    if (command == "CREATE") {
        status = createTable(arguments);
    }  else  if (command == "INSERT") {
        status = insertInTable(arguments);
    } else if (command == "DELETE") {
        status = deleteInTable(arguments);
    } else if (command == "SELECT") {
        status = selectFromTable(arguments);
    } else {
        std::cout << "Wrong command";
    }

    if (!status) {
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
    if (pos != -1) {
        std::cout << "Table " << parameters[0] << " already exists.\n";
        return false;
    }
    pos = m_tables.size();
    m_tables.push_back(Table(parameters[0]));

    parameters.erase(parameters.begin());
    std::vector<std::string> arguments;
    for (const auto& i : parameters) {
        if (i == "INDEXED") {
            m_tables[pos].setIndexedRow(arguments.size() - 1, arguments.back());
            continue;
        }
        arguments.push_back(i);
    }
    
    m_tables[pos].setCollumnNames(arguments);

    std::cout << "Table " << m_tables[pos].getName() << " created" << std::endl;
    return true;
}

bool db::insertInTable(std::vector<std::string>& parameters) 
{
    int pos = getTableIndex(parameters[0]);
    if (pos == -1) {
        std::cout << "Table " << parameters[0] << " does not exist." << std::endl;
        return false;
    }

    parameters.erase(parameters.begin());
    if (m_tables[pos].getCollumnCount() != parameters.size()) {
        std::cout << "Not consistent amount of parameters " << parameters.size() << std::endl;
        return false;
    }
    m_tables[pos].insertRow(parameters);

    std::cout << "New row inserted to " << m_tables[pos].getName() << std::endl;
    return true;
}

bool db::deleteInTable(std::vector<std::string>& parameters) 
{
    int pos = getTableIndex(parameters[0]);
    if (pos == -1) {
        std::cout << "Table " << parameters[0] << " not found" << std::endl;
        return false;
    }
    parameters.erase(parameters.begin());
    if (parameters.size() >= 1 && parameters[0] == "WHERE") {
        parameters.erase(parameters.begin());
        if (!m_tables[pos].deleteWithCondition(parameters)) {
            return false;
        }
    } else {
        m_tables[pos].deleteAllRows();
    }

    return true;
}

bool db::selectFromTable(std::vector<std::string>& parameters) 
{
    std::vector<std::string> collums;
    int pos = -1;
    if (parameters[0] == "*") {
        parameters.erase(parameters.begin());
        parameters.erase(parameters.begin());
        pos = getTableIndex(parameters[0]);
        if (pos == -1) {
            std::cout << "Table " << parameters[0] << " not found" << std::endl;
            return false;
        }
        parameters.erase(parameters.begin());
        collums = m_tables[pos].getCollumnNames();
    } else {
        for (const auto& param : parameters) {
            if (param == "FROM") {
                break;
            }
            collums.push_back(param);
        }

        for (int i = 0; i <= collums.size(); i++){
            parameters.erase(parameters.begin());
        }
        
        pos = getTableIndex(parameters[0]);
        if (pos == -1) {
            std::cout << "Table " << parameters[0] << " not found" << std::endl;
            return false;
        }
        parameters.erase(parameters.begin());
        for (const auto& collum : collums) {
            if (std::find(m_tables[pos].getCollumnNames().begin(), m_tables[pos].getCollumnNames().end(), collum) == m_tables[pos].getCollumnNames().end()){
                if (std::find(parameters.begin(), parameters.end(), "LEFT_JOIN") != parameters.end()){
                    continue;
                }
                std::cout << "There no collum " << collum << " in table " << m_tables[pos].getName() << std::endl;
                return false;
            }
        }
    }
    if (parameters.empty()) {
        if (collums.size() == m_tables[pos].getCollumnCount()) {
            m_tables[pos].printTable();
            return true;
        } else {
            m_tables[pos].printCollums(collums);
            return true;
        }
    } else if (parameters[0] == "WHERE") {
        parameters.erase(parameters.begin());
        std::vector<int> rows = m_tables[pos].getRowsFromIndexed(parameters[0], parameters[2]);
        if (!rows.empty()) {
            if (parameters[1] == "=") {
                m_tables[pos].printCollumsOnRows(collums, rows);
            } else {
                std::vector<int> newRows;
                for (auto i = 0; i < m_tables[pos].getRowCount(); i++) {
                    if (std::find(rows.begin(), rows.end(), i) != rows.end()) {
                        continue;
                    }
                    newRows.push_back(i);
                }
                m_tables[pos].printCollumsOnRows(collums, newRows);
            }
        }
        else {
            if (parameters[1] == "=") {
                for (int i = 0; i < m_tables[pos].getRowCount(); i++) {
                    if (m_tables[pos].getValue(i, m_tables[pos].getCollumnNameIndex(parameters[0])) == parameters[2]) {
                        rows.push_back(i);
                    }
                }
            } else {
                for (int i = 0; i < m_tables[pos].getRowCount(); i++) {
                    if (m_tables[pos].getValue(i, m_tables[pos].getCollumnNameIndex(parameters[0])) != parameters[2]) {
                        rows.push_back(i);
                    }
                }
            }
            m_tables[pos].printCollumsOnRows(collums, rows);
        }
    } else if (parameters[0] == "LEFT_JOIN") {
        parameters.erase(parameters.begin());
        int pos2 = getTableIndex(parameters[0]);
        if (pos2 == -1) {
            std::cout << "Table " << parameters[0] << " not found" << std::endl;
            return false;
        }
        parameters.erase(parameters.begin());
        std::vector<std::string> collums1;
        std::vector<std::string> collums2;

        for (const auto& collum : collums) {

            if (m_tables[pos].getCollumnNameIndex(collum) != -1) {
                collums1.push_back(collum);
                continue;
            }
            if (m_tables[pos2].getCollumnNameIndex(collum) != -1) {
                collums2.push_back(collum);
                continue;
            }
            std::cout << "Collumn " << collum << " not found in " << m_tables[pos].getName() << " or " << m_tables[pos2].getName() << std::endl;
            return false;
        }
        
        Table leftJoin("LEFT JOIN");
        leftJoin.setCollumnNames(collums);
        std::vector<std::string> result;
        for (const auto& row : m_tables[pos].getRows()){
            for (const auto& collumn : collums){
                result.push_back(" ");
            }
            for (const auto& collumn : collums1){
                int index = leftJoin.getCollumnNameIndex(collumn);
                if (index == -1) {
                    std::cout << "BAD INDEX" << std::endl; //DEBUG LOG
                }
                result[index] = row[m_tables[pos].getCollumnNameIndex(collumn)];
            }
            leftJoin.insertRow(result);
            result.clear();
        }
        int firstIndex = 0;
        int secondIndex = 0;

        if (m_tables[pos].getCollumnNameIndex(parameters[0]) != -1) {
            firstIndex = m_tables[pos].getCollumnNameIndex(parameters[0]);
            if (m_tables[pos2].getCollumnNameIndex(parameters[2])) {
                secondIndex = m_tables[pos2].getCollumnNameIndex(parameters[2]);
            } else {
                std::cout << "There no " << parameters[2] << " collumn in table " << m_tables[pos2].getName() << std::endl;
                return false;
            }
        } else if (m_tables[pos2].getCollumnNameIndex(parameters[0]) != -1) {
            secondIndex = m_tables[pos2].getCollumnNameIndex(parameters[0]);
            if (m_tables[pos].getCollumnNameIndex(parameters[2])) {
                firstIndex = m_tables[pos2].getCollumnNameIndex(parameters[2]);
            } else {
                std::cout << "There no " << parameters[2] << " collumn in table " << m_tables[pos2].getName() << std::endl;
                return false;
            }
        } else {
            std::cout << "There no " << parameters[0] << " collumn in table " << m_tables[pos2].getName() << " or " << m_tables[pos].getName() << std::endl;
            return false;
        }
        
        int count = std::min(m_tables[pos].getRowCount(), m_tables[pos2].getRowCount());
        
        if (parameters[1] == "=") { 
            for (int i = 0; i < count; i++) {
                if (m_tables[pos].getValue(i, firstIndex) == m_tables[pos2].getValue(i, secondIndex)) {
                    for (const auto& column : collums2) {
                        leftJoin.setValue(i, leftJoin.getCollumnNameIndex(column), m_tables[pos2].getValue(i, secondIndex));
                    }
                }
            }
        } else if (parameters[1] == "!=") {
            for (int i = 0; i < count; i++) {
                if (m_tables[pos].getValue(i, firstIndex) != m_tables[pos2].getValue(i, secondIndex)) {
                    for (const auto& column : collums2) {
                        leftJoin.setValue(i, leftJoin.getCollumnNameIndex(column), m_tables[pos2].getValue(i, secondIndex));
                    }
                }
            }
        }
        parameters.erase(parameters.begin());
        parameters.erase(parameters.begin());
        parameters.erase(parameters.begin());
        if (!parameters.empty()) {
            std::vector<int> rows;
            if (parameters[1] == "=") {
                for (int i = 0; i < leftJoin.getRowCount(); i++) {
                    if (leftJoin.getValue(i, leftJoin.getCollumnNameIndex(parameters[0])) == parameters[2]) {
                        rows.push_back(i);
                    }
                }
            } else {
                for (int i = 0; i < leftJoin.getRowCount(); i++) {
                    if (leftJoin.getValue(i, leftJoin.getCollumnNameIndex(parameters[0])) != parameters[2]) {
                        rows.push_back(i);
                    }
                }
            }
            leftJoin.printCollumsOnRows(collums, rows);
        } else {
            leftJoin.printTable();
        }
    }

    return true;
}

void db::printTables() 
{
    for (const auto& i : m_tables){
        i.printTable();
    }
}

Table::Table() 
    : m_collumnCount(0)
    , m_collumnNames()
    , m_name("")
    , m_rows()
    , m_indexedRows()
{}

Table::Table(const std::string& name) 
    : m_collumnCount(0)
    , m_collumnNames()
    , m_name(name)
    , m_rows()
    , m_indexedRows()
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
    if (row.size() != m_collumnCount) {
        std::cout << "DEBUG: WRONG LENGTH OF PARAMENTER row IN insertRow" << std::endl;
    }
    m_rows.push_back(row);
    int row_index = m_rows.size() - 1;
    for (const auto& indexed : m_indexedRows) {
        const std::string& columnName = indexed.first;
        int columnIndex = getCollumnNameIndex(columnName);   
        auto tree = indexed.second;
        if (tree.find(row[columnIndex]) != tree.end()) {
            tree[row[columnIndex]].push_back(row_index);
        } else {
            tree.emplace(row[columnIndex], std::vector<int>(row_index));
        }
    }
}

void Table::setCollumnNames(const std::vector<std::string>& names) 
{
    m_collumnNames = names;
    m_collumnCount = names.size();
}

void Table::deleteAllRows() 
{
    m_rows.clear();
    m_indexedRows.clear();
}

int Table::getCollumnNameIndex(const std::string& name) 
{
    for (int i = 0; i < m_collumnCount; i++) {
        if (m_collumnNames[i] == name) return i;
    }
    return -1;
}


void Table::deleteFromIndexed(int rowIndex) {
    for (const auto& indexed : m_indexedRows) {
        int columnIndex = getCollumnNameIndex(indexed.first);   
        auto tree = indexed.second;
        auto vec = tree[m_rows[rowIndex][columnIndex]];
        if (vec.size() > 1) {
            vec.erase(std::remove(vec.begin(), vec.end(), rowIndex), vec.end());
        } else {
            tree.erase(tree.find(m_rows[rowIndex][columnIndex]) , tree.end());
        }
    }
}

std::vector<std::string> Table::getCollumnNames() 
{
    return m_collumnNames;
}

void Table::printCollums(std::vector<std::string> collums) 
{
    std::cout << "\t" << m_name << "\t" << std::endl;
    printHorizontalLane(m_collumnCount*15);
    for (const auto& i : collums){
        std::cout << i << '\t' << '|';
    }
    std::cout << std::endl;
    printHorizontalLane(m_collumnCount*15);
    for (const auto& row: m_rows){
        for (const auto& collum : collums) {
            std::cout << row[getCollumnNameIndex(collum)] << '\t' << '|';
        }
        std::cout << std::endl;
        printHorizontalLane(m_collumnCount*15);
    }
}

std::vector<int> Table::getRowsFromIndexed(std::string collumName, std::string vallue) 
{
    if (m_indexedRows.find(collumName) != m_indexedRows.end()) {
        return m_indexedRows[collumName][vallue];
    }
    return std::vector<int>();
}

void Table::printCollumsOnRows(std::vector<std::string> collums, std::vector<int> rows) 
{
    std::cout << "\t" << m_name << "\t" << std::endl;
    printHorizontalLane(m_collumnCount*15);
    for (const auto& i : collums){
        std::cout << i << '\t' << '|';
    }
    std::cout << std::endl;
    printHorizontalLane(m_collumnCount*15);
    for (const auto& row: rows){
        for (const auto& collum : collums) {
            std::cout << m_rows[row][getCollumnNameIndex(collum)] << '\t' << '|';
        }
        std::cout << std::endl;
        printHorizontalLane(m_collumnCount*15);
    }
}

int Table::getRowCount() 
{
    return m_rows.size();
}

std::vector<std::vector<std::string>>& Table::getRows() 
{
    return m_rows;
}

void Table::insertVallue(int ypos, int xpos, std::string vallue) 
{
    if (ypos < m_rows.size() && xpos < m_collumnNames.size()) {
        m_rows[ypos][xpos] = vallue;
    }
}

std::string Table::getValue(int ypos, int xpos) 
{
    if (ypos < m_rows.size() && xpos < m_collumnNames.size()) {
        return m_rows[ypos][xpos];
    }
    std::cout << "Wrong index in getValue" << std::endl;
    return std::string();
}

void Table::setValue(int ypos, int xpos, std::string value) 
{
    if (ypos < m_rows.size() && xpos < m_collumnNames.size()) {
        m_rows[ypos][xpos] = value;
        return;
    }
    std::cout << "Wrong index in setValue" << std::endl;
}

bool Table::deleteWithCondition(const std::vector<std::string>& condition) 
{
    int pos = getCollumnNameIndex(condition[0]);
    if (pos == -1) {
        std::cout << "There no column " << condition[0] << " in table " << m_name << std::endl;
        return false;
    }
    int size = m_rows.size()-1;
    if (condition[1] == "=") {
        for (int i = size; i >= 0; i--){
            if (m_rows[i][pos].find(condition[2]) != std::string::npos) {
                deleteFromIndexed(i);
                m_rows.erase(m_rows.begin() + i);
            }
        }
    } else if (condition[1] == "!=") {
        for (int i = size; i >= 0; i--){
            if (m_rows[i][pos].find(condition[2]) == std::string::npos) {
                deleteFromIndexed(i);
                m_rows.erase(m_rows.begin() + i);
            }
        }
    }
    return true;
}

void Table::printTable() const
{
    std::cout << "\t" << m_name << "\t" << std::endl;
    printHorizontalLane(m_collumnCount*15);
    for (const auto& i : m_collumnNames){
        std::cout << i << '\t' << '|';
    }
    std::cout << std::endl;
    printHorizontalLane(m_collumnCount*15);
    for (const auto& row: m_rows){
        for (const auto& i : row){
            std::cout << i << '\t' << '|';
        }
        std::cout << std::endl;
        printHorizontalLane(m_collumnCount*15);
    }
}

void Table::setIndexedRow(int index, std::string name) 
{
    m_indexedRows.emplace(name, std::map<std::string, std::vector<int>>());
}

db::db()
    : m_tables()
{
    
}
