#include "parser.h"
    
#include <iostream>

Parser::Parser(std::string input) 
    : m_lexer(Lexer(input))  
{}


std::vector<std::string> Parser::getCreateCommand() {
    std::vector<std::string> output;
    output.push_back("CREATE");
    Token token(m_lexer.getNextToken());

    if (token.getType() == TokenType::NAME) {
        output.push_back(token.getValue());    
        token = m_lexer.getNextToken();
    } else {
        errorWrongSymbol("NAME", token.getValue());
        return std::vector<std::string>();
    }

    if (token.getType() == TokenType::OPEN_PARENTHESIS) {
        token = m_lexer.getNextToken();
    } else {
        errorWrongSymbol("(", token.getValue());
        return std::vector<std::string>();
    }
    do {
        if (token.getType() == TokenType::NAME) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();
            if (token.getType() == TokenType::INDEXED) {
                output.push_back(token.getValue());
                token = m_lexer.getNextToken();
            }
        }

        if (token.getType() == TokenType::COMMA) {
            token = m_lexer.getNextToken();
            continue;
        }
        
        if (token.getType() != TokenType::CLOSE_PARENTHESIS) {
            errorWrongSymbol("NAME or COMMA", token.getValue());
            return std::vector<std::string>();
        } 

    } while (token.getType() != TokenType::CLOSE_PARENTHESIS);

    token = m_lexer.getNextToken();
    if (token.getType() == TokenType::STOP) {
        return output;
    }
    errorWrongSymbol(";", token.getValue());
    return std::vector<std::string>();
}

std::vector<std::string> Parser::getInsertCommannd() 
{
    std::vector<std::string> output;
    output.push_back("INSERT");
    Token token(m_lexer.getNextToken());

    if (token.getType() == TokenType::INTO) {
        token = m_lexer.getNextToken();
    }

    if (token.getType() == TokenType::NAME) {
        output.push_back(token.getValue());
        token = m_lexer.getNextToken();
    } else {
        errorWrongSymbol("NAME", token.getValue());
        return std::vector<std::string>();
    }

    if (token.getType() == TokenType::OPEN_PARENTHESIS) {
        token = m_lexer.getNextToken();
    } else {
        errorWrongSymbol("(", token.getValue());
        return std::vector<std::string>();
    }

    do {

        if (token.getType() == TokenType::VALUE) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();
        }

        if (token.getType() == TokenType::COMMA) {
            token = m_lexer.getNextToken();
            continue;
        }
        if (token.getType() != TokenType::CLOSE_PARENTHESIS) {
            errorWrongSymbol("VALLUE or COMMA", token.getValue());
            return std::vector<std::string>();
        } 

    } while (token.getType() != TokenType::CLOSE_PARENTHESIS);

    token = m_lexer.getNextToken();
    
    if (token.getType() == TokenType::STOP) {
        return output;
    }
    errorWrongSymbol(";", token.getValue());
    return std::vector<std::string>();
}

std::vector<std::string> Parser::getSelectCommannd() 
{
    std::vector<std::string> output;
    output.push_back("SELECT");
    Token token(m_lexer.getNextToken());
    bool isLeftJoinRestricted = false;

    if (token.getType() == TokenType::ASTERIX) {
        output.push_back(token.getValue());
        token = m_lexer.getNextToken();
        if (token.getType() == TokenType::FROM) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("FROM", token.getValue());
            return std::vector<std::string>();
        }
    } else if (token.getType() == TokenType::NAME) {
        do {
            if (token.getType() == TokenType::NAME) {
                output.push_back(token.getValue());
                token = m_lexer.getNextToken();
            }

            if (token.getType() == TokenType::COMMA) {
                token = m_lexer.getNextToken();
                continue;
            }
            if (token.getType() != TokenType::FROM) {
                errorWrongSymbol("NAME or COMMA", token.getValue());
                return std::vector<std::string>();
            } 

        } while (token.getType() != TokenType::FROM);
        output.push_back(token.getValue());
        token = m_lexer.getNextToken();
    } else {
        errorWrongSymbol("NAME or ASTERIX", token.getValue());
        return std::vector<std::string>();
    }

    if (token.getType() == TokenType::NAME) {
        output.push_back(token.getValue());
        token = m_lexer.getNextToken();
    } else {
        errorWrongSymbol("NAME", token.getValue());
        return std::vector<std::string>();
    }

    if (token.getType() == TokenType::LEFT_JOIN) {
        output.push_back(token.getValue());
        token = m_lexer.getNextToken();
        
        if (token.getType() == TokenType::NAME) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("NAME", token.getValue());
            return std::vector<std::string>();
        }

        if (token.getType() == TokenType::ON) {
            token = m_lexer.getNextToken();

            if (token.getType() == TokenType::NAME) {
                output.push_back(token.getValue());
                token = m_lexer.getNextToken();
            } else {
                errorWrongSymbol("NAME", token.getValue());
                return std::vector<std::string>();
            }
            if (token.getType() == TokenType::SIGN) {
                output.push_back(token.getValue());
                token = m_lexer.getNextToken();
            } else {
                errorWrongSymbol("SIGN", token.getValue());
                return std::vector<std::string>();
            }
            if (token.getType() == TokenType::NAME) {
                output.push_back(token.getValue());
                token = m_lexer.getNextToken();
            } else {
                errorWrongSymbol("NAME", token.getValue());
                return std::vector<std::string>();
            }
            if (token.getType() == TokenType::STOP) {
                return output;
            }
        } else {
            errorWrongSymbol("ON", token.getValue());
            return std::vector<std::string>();
        }
    } else if (token.getType() == TokenType::WHERE) {
        output.push_back(token.getValue());
        isLeftJoinRestricted = true;
    }

    if (token.getType() == TokenType::WHERE) {
        token = m_lexer.getNextToken();

        if (token.getType() == TokenType::NAME) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("NAME", token.getValue());
            return std::vector<std::string>();
        }
        if (token.getType() == TokenType::SIGN) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("SIGN", token.getValue());
            return std::vector<std::string>();
        }
        if (token.getType() == TokenType::VALUE) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("VALUE", token.getValue());
            return std::vector<std::string>();
        }
        if (token.getType() == TokenType::LEFT_JOIN && isLeftJoinRestricted) {
            std::cout << "LEFT_JOIN after WHERE" << std::endl;
            return std::vector<std::string>();
        }
        if (token.getType() == TokenType::STOP) {
            return output;
        } else {
            errorWrongSymbol(";", token.getValue());
            return std::vector<std::string>();
        }
    }

    if (token.getType() == TokenType::STOP) {
        return output;
    }
    errorWrongSymbol(";", token.getValue());
    return std::vector<std::string>();

}

std::vector<std::string> Parser::getDeleteCommannd() 
{
    std::vector<std::string> output;
    output.push_back("DELETE");
    Token token(m_lexer.getNextToken());

    if (token.getType() == TokenType::FROM) {
        output.push_back(token.getValue());
        token = m_lexer.getNextToken();
    }
    if (token.getType() == TokenType::NAME) {
        output.push_back(token.getValue());
        token = m_lexer.getNextToken();
    } else {
        errorWrongSymbol("NAME", token.getValue());
    }
    if (token.getType() == TokenType::WHERE) {
        output.push_back(token.getValue());
        token = m_lexer.getNextToken();

        if (token.getType() == TokenType::NAME) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("NAME", token.getValue());
        }
        if (token.getType() == TokenType::SIGN) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("SIGN", token.getValue());
        }
        if (token.getType() == TokenType::VALUE) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("VALUE", token.getValue());
        }
    }
    if (token.getType() == TokenType::STOP) {
            return output;
        }
    errorWrongSymbol(";", token.getValue());
    return std::vector<std::string>();

}

std::vector<std::string> Parser::getNextCommand() 
{
    Token token(m_lexer.getNextToken());
    std::vector<std::string> arguments{};

    switch (token.getType()) {
        case TokenType::CREATE:
            arguments = getCreateCommand();
            break;
        case TokenType::INSERT:
            arguments = getInsertCommannd();
            break;
        case TokenType::SELECT:
            arguments = getSelectCommannd();
            break;
        case TokenType::DELETE:
            arguments = getDeleteCommannd();
            break;
        default:
            std::cout << "Wrong command" << std::endl;
            break;
    }
    return arguments;

}

void Parser::errorWrongSymbol(std::string waiting, std::string actual) 
{
    if (waiting == actual){
        abort();
    } else {
        std::cout << "Waiting for \'" << waiting <<"\' but \'" << actual << "\' received" << std::endl;
    }
}