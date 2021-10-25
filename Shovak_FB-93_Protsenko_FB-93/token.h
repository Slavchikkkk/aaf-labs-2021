#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

enum TokenType {
    UNDEFINED = -1,
    CREATE,
    INDEXED,
    INSERT,
    INTO,
    SELECT,
    FROM,
    LEFT_JOIN,
    ON,
    WHERE,    
    DELETE,
    NAME,
    VALUE,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    SIGN,
    COMMA,
    ASTERIX,
    STOP,
    ERROR
};


class Token {
    public:
    Token(TokenType type, const std::string& value);
    Token();

    TokenType getType();
    std::string& getValue();
    bool isEmpty();
    friend std::ostream& operator<<(std::ostream& os, const Token& token);
    
    private:
    TokenType m_type;
    std::string m_value;
};
#endif // __TOKEN_H__