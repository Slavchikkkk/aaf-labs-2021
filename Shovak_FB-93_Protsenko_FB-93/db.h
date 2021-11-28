#ifndef __DB_H__
#define __DB_H__

#include "parser.h"
#include <string>
#include <vector>

class Table{
    public:
    Table();
    Table(const std::string& name);
    std::string getName();
    int getCollumnCount();
    void insertRow(const std::vector<std::string>& row);
    void setCollumnNames(const std::vector<std::string>& names);
    void deleteAllRows();
    int getCollumnNameIndex(const std::string& name);
    void deleteWithCondition(const std::vector<std::string>& condition);
    void printTable();
    private:
    std::vector<std::string> m_collumnNames;
    std::string m_name;
    int m_collumnCount;
    std::vector<std::vector<std::string>> m_rows;
};


class db {
    public:
    db();
    void executeCommand(std::string input);
    int getTableIndex(const std::string& name);
    void printTables();
    private:
    bool createTable(std::vector<std::string>& parameters);
    bool insertInTable(std::vector<std::string>& parameters);
    bool deleteInTable(std::vector<std::string>& parameters);
    private:
    std::vector<Table> m_tables;
};




 #endif // __DB_H__