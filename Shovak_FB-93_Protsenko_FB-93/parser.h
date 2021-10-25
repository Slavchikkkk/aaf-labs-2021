#ifndef __PARSER_H__
#define __PARSER_H__

#include "lexer.h"
#include <string>
#include <deque>

class Parser {
    public:
    Parser(std::string input);

    std::deque<std::string> getCreateCommand();
    std::deque<std::string> getInsertCommannd();
    std::deque<std::string> getSelectCommannd();
    std::deque<std::string> getDeleteCommannd();

    void getNextCommand();
    
    private:
    void errorWrongSymbol(std::string waiting, std::string actual);
    
    private:
    Lexer m_lexer;
};

#endif // __PARSER_H__