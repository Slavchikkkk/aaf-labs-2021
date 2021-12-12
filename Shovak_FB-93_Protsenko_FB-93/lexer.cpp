#include "lexer.h"

#include <algorithm>
#include <iostream>
//Helper funcs

Lexer::Lexer(const std::string& input)
    : m_input(input)
    , m_currentIndex(0)
    , m_currentChar(m_input[m_currentIndex])
{}

Token Lexer::getNextToken() 
{    
    while (true) {
        if (m_currentChar == ' ' || m_currentChar == '\t' || m_currentChar == '\r' || m_currentChar == '\n'){
            skipToNextChar();
            continue;
        } else if (isalpha(m_currentChar)) {
            return tokenFromString();
        } else if (m_currentChar == '(') {
            skipToNextChar();
            return Token(TokenType::OPEN_PARENTHESIS, std::string(1, m_input[m_currentIndex - 1]));
        } else if (m_currentChar == ')') {
            skipToNextChar();
            return Token(TokenType::CLOSE_PARENTHESIS, std::string(1, m_input[m_currentIndex - 1]));
        } else if (m_currentChar == '=' || m_currentChar == '!' || m_currentChar == '<' || m_currentChar == '>') {
            return tokenFromSign();
        } else if (m_currentChar == ',') {
            skipToNextChar();
            return Token(TokenType::COMMA, std::string(1, m_input[m_currentIndex - 1]));
        } else if (m_currentChar == '\"') {
            return tokenFromValue();
        } else if (m_currentChar == '*') {
            skipToNextChar();
            return Token(TokenType::ASTERIX, std::string(1, m_input[m_currentIndex - 1])); 
        } else if (m_currentChar == ';') {
            return Token(TokenType::STOP, std::string(1, m_currentChar));
        }
        return Token(TokenType::ERROR, std::string("Wrong symbol \'" +  std::string(1, m_currentChar) + "\' was found"));
    }
    return Token(TokenType::ERROR, std::string("Wrong symbol \'" + std::string(1, m_currentChar) + "\' was found"));
}

void Lexer::skipToNextChar() 
{
    m_currentIndex++;
    if (m_currentIndex < m_input.length()) { 
        m_currentChar = m_input[m_currentIndex];
    } else {
        std::cout << "Index is higher then length";
    }
}

Token Lexer::tokenFromString() 
{
    std::string result = "";
    while (isalpha(m_currentChar) || isdigit(m_currentChar) || m_currentChar == '_')
    {
        result.push_back(m_currentChar);
        skipToNextChar();
        if (m_currentIndex >= m_input.size()) {
            return Token(TokenType::ERROR, "Out on bound");
        }
    }

    if (result == "CREATE") {
        return Token(TokenType::CREATE, result);
    } else if (result == "INDEXED") {
        return Token(TokenType::INDEXED, result);
    } else if (result == "INSERT") {
        return Token(TokenType::INSERT, result);
    } else if (result == "INTO") {
        return Token(TokenType::INTO, result);
    } else if (result == "SELECT") {
        return Token(TokenType::SELECT, result);
    } else if (result == "FROM") {
        return Token(TokenType::FROM, result);
    } else if (result == "LEFT_JOIN") {
        return Token(TokenType::LEFT_JOIN, result);
    } else if (result == "ON") {
        return Token(TokenType::ON, result);
    } else if (result == "WHERE") {
        return Token(TokenType::WHERE, result);
    } else if (result == "DELETE") {
        return Token(TokenType::DELETE, result);
    } else {
        return Token(TokenType::NAME, result);
    }

    return  Token(TokenType::ERROR, "wrong name entered");
}

Token Lexer::tokenFromValue() 
{
    std::string result = "";
    bool close = true;
    while (close) {
        skipToNextChar();
        if (m_currentChar == '\"') {
            skipToNextChar();
            close = false;
            continue;
        }
        result.push_back(m_currentChar);
    }
    return Token(TokenType::VALUE, result);
}

Token Lexer::tokenFromSign() 
{
    std::string result = "";
    result.push_back(m_currentChar);
    switch (m_currentChar)
    {
    case '=':
        skipToNextChar();
        break;
    case '!':
        skipToNextChar();
        if (m_currentChar == '='){
            result.push_back(m_currentChar);
            skipToNextChar();
        } else {
            return Token(TokenType::ERROR, std::string("Not found \'=\' after \'!\', but \'" + std::string(1, m_currentChar) + "\' was found"));
        }
        break;
    case '<'||'>':
        skipToNextChar();
        if (m_currentChar == '='){
            result.push_back(m_currentChar);
            skipToNextChar();
        }
        break;
    default:
        return Token(TokenType::ERROR, std::string("Wrong character" + std::string(1, m_currentChar) + "\' was found"));
        break;
    }
    
    return Token(TokenType::SIGN, result);
}