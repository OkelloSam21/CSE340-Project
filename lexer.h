#ifndef LEXER_H
#define LEXER_H

#include <stdexcept>
#include <string>

enum TokenType {
    END_OF_FILE,
    PUBLIC,
    PRIVATE,
    EQUAL,
    COLON,
    COMMA,
    SEMICOLON,
    LBRACE,
    RBRACE,
    ID,
    INTEGER_LITERAL,
    STRING_LITERAL,
    ERROR,
};

struct Token {
    TokenType type;
    std::string value;

    Token() : type(END_OF_FILE), value("") {}
    Token(TokenType t, const std::string& v) : type(t), value(v) {}
};

class Lexer {
private:
    std::string input;
    size_t position = 0;

    void skipWhitespace();
    void skipComments();
    Token lexIdentifier();
    Token lexIntegerLiteral();
    Token lexStringLiteral();

public:
    explicit Lexer(const std::string& src);
    Token getNextToken();
};

#endif // LEXER_H
