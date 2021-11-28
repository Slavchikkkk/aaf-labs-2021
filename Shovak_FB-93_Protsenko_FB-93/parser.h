#ifndef __PARSER_H__
#define __PARSER_H__

#include "lexer.h"
#include <string>
#include <vector>

class Parser {
    public:
    Parser(std::string input);

   
    std::vector<std::string> getNextCommand();
    
    private:
    std::vector<std::string> getCreateCommand();
    std::vector<std::string> getInsertCommannd();
    std::vector<std::string> getSelectCommannd();
    std::vector<std::string> getDeleteCommannd();

    void errorWrongSymbol(std::string waiting, std::string actual);
    
    private:
    Lexer m_lexer;
};

#endif // __PARSER_H__