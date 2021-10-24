#ifndef __PARSER_H__
#define __PARSER_H__

#include "lexer.h"
#include <string>
#include <deque>

struct createCommandClass
{
    int indexed = 0;
    std::deque<std::string> rows{};
    std::string name = "";
};

struct insertCommandClass
{
    bool into = false;
    std::deque<std::string> rows{};
    std::string name = "";
};

struct selectCommandClass
{
    std::deque<std::string> rows{};
    std::string fromWhere = "";
    std::string lvalue = "";
    std::string rvalue = "";
    std::string sing = "";
    std::string leftJoinName = "";
     
};
struct deleteCommandClass
{
    bool from = false;
    bool where = false;
    std::string name = "";
    std::string lvalue = "";
    std::string rvalue = "";
    std::string sing = "";
};

class Parser {
    public:
    Parser(std::string input);

    std::deque<std::string> getCreateCommand();
    std::deque<std::string> getInsertCommannd();
    std::deque<std::string> getSelectCommannd();
    deleteCommandClass getDeleteCommannd();
    void getNextCommand();
    private:
    void errorWrongSymbol(std::string waiting, std::string actual);
    private:
    Lexer m_lexer;
};

#endif // __PARSER_H__