#ifndef __LEXER_H__
#define __LEXER_H__

#include <string>

#include "token.h"


class Lexer {
    public:
    Lexer(const std::string& input);

    Token getNextToken();

    private:
    void skipToNextChar();
    Token tokenFromString();
    Token tokenFromValue();
    Token tokenFromSign();
    
    private:
    std::string m_input;
    int m_currentIndex;
    char m_currentChar;
};

#endif // __LEXER_H__