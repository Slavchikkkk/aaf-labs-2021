#include "parser.h"

#include <iostream>

Parser::Parser(std::string input) 
    : m_lexer(Lexer(input))  
{}
void printDequue(const std::deque<std::string>& output){
    for (auto i = output.begin(); i < output.end(); i++){
        std::cout << *i << " ";
    }
}
std::deque<std::string> Parser::getCreateCommand() {
    std::deque<std::string> output;
    output.push_back("CREATE");
    Token token(m_lexer.getNextToken());

    if (token.getType() == TokenType::NAME) {
        output.push_back(token.getValue());    
        token = m_lexer.getNextToken();
    } else {
        errorWrongSymbol("NAME", token.getValue());
        return std::deque<std::string>();
    }

    if (token.getType() == TokenType::OPEN_PARENTHESIS) {
        token = m_lexer.getNextToken();
    } else {
        errorWrongSymbol("(", token.getValue());
        return std::deque<std::string>();
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
            return std::deque<std::string>();
        } 

    } while (token.getType() != TokenType::CLOSE_PARENTHESIS);

    token = m_lexer.getNextToken();
    if (token.getType() == TokenType::STOP) {
        printDequue(output);
        return output;
    }
    errorWrongSymbol(";", token.getValue());
    return std::deque<std::string>();
}

std::deque<std::string> Parser::getInsertCommannd() 
{
    std::deque<std::string> output;
    output.push_back("INSERT");
    Token token(m_lexer.getNextToken());

    if (token.getType() == TokenType::INTO) {
        output.push_back(token.getValue());
        token = m_lexer.getNextToken();
    }

    if (token.getType() == TokenType::NAME) {
        output.push_back(token.getValue());
        token = m_lexer.getNextToken();
    } else {
        errorWrongSymbol("NAME", token.getValue());
        return std::deque<std::string>();
    }

    if (token.getType() == TokenType::OPEN_PARENTHESIS) {
        token = m_lexer.getNextToken();
    } else {
        errorWrongSymbol("(", token.getValue());
        return std::deque<std::string>();
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
            return std::deque<std::string>();
        } 

    } while (token.getType() != TokenType::CLOSE_PARENTHESIS);

    token = m_lexer.getNextToken();
    
    if (token.getType() == TokenType::STOP) {
        printDequue(output);
        return output;
    }
    errorWrongSymbol(";", token.getValue());
    return std::deque<std::string>();
}

std::deque<std::string> Parser::getSelectCommannd() 
{
    std::deque<std::string> output;
    output.push_back("SELECT");
    Token token(m_lexer.getNextToken());
    bool isLeftJoinRestricted = false;

    if (token.getType() == TokenType::ASTERIX) {
        output.push_back(token.getValue());
        token = m_lexer.getNextToken();
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
                return std::deque<std::string>();
            } 

        } while (token.getType() != TokenType::FROM);
        output.push_back(token.getValue());
        token = m_lexer.getNextToken();
    } else {
        errorWrongSymbol("NAME or ASTERIX", token.getValue());
        return std::deque<std::string>();
    }

    if (token.getType() == TokenType::NAME) {
        output.push_back(token.getValue());
        token = m_lexer.getNextToken();
    } else {
        errorWrongSymbol("NAME", token.getValue());
        return std::deque<std::string>();
    }

    if (token.getType() == TokenType::LEFT_JOIN) {
        output.push_back(token.getValue());
        token = m_lexer.getNextToken();
        
        if (token.getType() == TokenType::NAME) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("NAME", token.getValue());
            return std::deque<std::string>();
        }

        if (token.getType() == TokenType::ON) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();

            if (token.getType() == TokenType::NAME) {
                output.push_back(token.getValue());
                token = m_lexer.getNextToken();
            } else {
                errorWrongSymbol("NAME", token.getValue());
                return std::deque<std::string>();
            }
            if (token.getType() == TokenType::SIGN) {
                output.push_back(token.getValue());
                token = m_lexer.getNextToken();
            } else {
                errorWrongSymbol("SIGN", token.getValue());
                return std::deque<std::string>();
            }
            if (token.getType() == TokenType::NAME) {
                output.push_back(token.getValue());
                token = m_lexer.getNextToken();
            } else {
                errorWrongSymbol("NAME", token.getValue());
                return std::deque<std::string>();
            }
            if (token.getType() == TokenType::STOP) {
                printDequue(output);
                return output;
            } else {
                errorWrongSymbol(";", token.getValue());
                return std::deque<std::string>();
            }
        } else {
            errorWrongSymbol("ON", token.getValue());
            return std::deque<std::string>();
        }
    } else if (token.getType() == TokenType::WHERE) {
        isLeftJoinRestricted = true;
    }

    if (token.getType() == TokenType::WHERE) {
        if (!isLeftJoinRestricted) {
            std::cout << "LEFT_JOIN afrer WHERE";
            return std::deque<std::string>();
        }
        token = m_lexer.getNextToken();

        if (token.getType() == TokenType::NAME) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("NAME", token.getValue());
            return std::deque<std::string>();
        }
        if (token.getType() == TokenType::SIGN) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("SIGN", token.getValue());
            return std::deque<std::string>();
        }
        if (token.getType() == TokenType::VALUE) {
            output.push_back(token.getValue());
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("VALUE", token.getValue());
            return std::deque<std::string>();
        }
        if (token.getType() == TokenType::STOP) {
            printDequue(output);
            return output;
        } else {
            errorWrongSymbol(";", token.getValue());
            return std::deque<std::string>();
        }
    }

    if (token.getType() == TokenType::STOP) {
        printDequue(output);
        return output;
    }
    errorWrongSymbol(";", token.getValue());
    return std::deque<std::string>();

}

deleteCommandClass Parser::getDeleteCommannd() 
{
    Token token(m_lexer.getNextToken());
    deleteCommandClass output;

    if (token.getType() == TokenType::FROM) {
        output.from = true;
        token = m_lexer.getNextToken();
    }
    if (token.getType() == TokenType::NAME) {
        output.name = token.getValue();
        token = m_lexer.getNextToken();
    } else {
        errorWrongSymbol("NAME", token.getValue());
    }
    if (token.getType() == TokenType::WHERE) {
        output.where = true;
        token = m_lexer.getNextToken();

        if (token.getType() == TokenType::NAME) {
            output.lvalue = token.getValue();
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("NAME", token.getValue());
        }
        if (token.getType() == TokenType::SIGN) {
            output.sing = token.getValue();
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("SIGN", token.getValue());
        }
        if (token.getType() == TokenType::VALUE) {
            output.rvalue = token.getValue();
            token = m_lexer.getNextToken();
        } else {
            errorWrongSymbol("VALUE", token.getValue());
        }
    }
    if (token.getType() == TokenType::STOP) {
            std::cout << "Name: " << output.name << " Where: " << output.where;
            return output;
        } else {
            errorWrongSymbol(";", token.getValue());
            return deleteCommandClass();
        }
}

void Parser::getNextCommand() 
{
    Token token(m_lexer.getNextToken());
    std::deque<std::string> arguments{};
    switch (token.getType()) {
        case TokenType::CREATE:
            getCreateCommand();
            break;
        case TokenType::INSERT:
            getInsertCommannd();
            break;
        case TokenType::SELECT:
            getSelectCommannd();
            break;
        case TokenType::DELETE:
            getDeleteCommannd();
            break;
        default:
            std::cout << "wrong command";
            break;
    }
    if (arguments.empty()){
        std::cout << "ERROR";
    }
}

void Parser::errorWrongSymbol(std::string waiting, std::string actual) 
{
    if (waiting == actual){
        abort();
    } else {
        std::cout << "Waiting for \'" << waiting <<"\' but \'" << actual << "\' received" << std::endl;
    }
}