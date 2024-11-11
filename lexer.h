#ifndef LEXER_H
#define LEXER_H

#include <stdexcept>
#include <string>

enum TokenType {
    END_OF_FILE = 0,
    PUBLIC,
    PRIVATE,
    EQUAL,
    COLON,
    COMMA,
    SEMICOLON,
    LBRACE,
    RBRACE,
    ID,
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

public:
    explicit Lexer(const std::string& src);
    Token getNextToken();
};

#endif // LEXER_H
