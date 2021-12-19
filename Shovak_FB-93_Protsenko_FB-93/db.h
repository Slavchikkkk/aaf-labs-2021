#ifndef __DB_H__
#define __DB_H__

#include "parser.h"
#include <string>
#include <vector>
#include <map>

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
    bool deleteWithCondition(const std::vector<std::string>& condition);
    void printTable() const;
    void setIndexedRow(int index, std::string name);
    void deleteFromIndexed(int rowIndex);
    std::vector<std::string> getCollumnNames();
    void printCollums(std::vector<std::string> collums);
    std::vector<int> getRowsFromIndexed(std::string collumName, std::string vallue);
    void printCollumsOnRows(std::vector<std::string> collums, std::vector<int> rows);
    int getRowCount();
    std::vector<std::vector<std::string>>& getRows();
    void insertVallue(int ypos, int xpos, std::string vallue);
    std::string getValue(int ypos, int xpos);
    void setValue(int ypos, int xpos, std::string value);
    private:
    std::vector<std::string> m_collumnNames;
    std::string m_name;
    int m_collumnCount;
    std::vector<std::vector<std::string>> m_rows;
    std::map<std::string, std::map<std::string, std::vector<int>>> m_indexedRows;
};

class db {
    public:
    db();
    void executeCommand(std::string input);
    private:
    int getTableIndex(const std::string& name);
    void printTables();
    private:
    bool createTable(std::vector<std::string>& parameters);
    bool insertInTable(std::vector<std::string>& parameters);
    bool deleteInTable(std::vector<std::string>& parameters);
    bool selectFromTable(std::vector<std::string>& parameters);
    private:
    std::vector<Table> m_tables;
};

 #endif // __DB_H__