#include "token.h"
#include <iostream>

Token::Token(TokenType type, const std::string& value) 
    : m_type(type)
    , m_value(value)
{}

Token::Token()
    : m_type(TokenType::UNDEFINED)
    , m_value(std::string())
{}

TokenType Token::getType() 
{
    return m_type;
}

std::string& Token::getValue() 
{
    return m_value;
}

std::ostream& operator<<(std::ostream& os, const Token& token) 
{
    os << "Type: " << token.m_type << " " << "Value: " << token.m_value << std::endl;
    return os;
}
