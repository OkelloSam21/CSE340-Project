#include "lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& src) : input(src) {}

void Lexer::skipWhitespace() {
    while (position < input.size() && isspace(input[position])) {
        position++;
    }
}

void Lexer::skipComments() {
    if(input[position] == '/' && position + 1 < input.size()) {
        if (input[position +  1] == '/')
        {
            position += 2;
            while (position < input.size() && input[position] != '\n')
            {
                position++;
            }
            position++;            
        }         
    }
}

Token Lexer::lexIdentifier() {
    std::string identifier;
    while (position < input.size() && (isalnum(input[position]) || input[position] == '_')) {
        identifier += input[position++];
    }

    if (identifier == "public") {
        return { PUBLIC, "public" };
    } else if (identifier == "private") {
        return { PRIVATE, "private" };
    } else {
        return { ID, identifier };
    }
}

Token Lexer::lexIntegerLiteral() {
    std::string integerLiteral;
    while (position < input.size() && isdigit(input[position])) {
        integerLiteral += input[position++];
    }
    return { INTEGER_LITERAL, integerLiteral };
}

Token Lexer::lexStringLiteral() {
    position++;
    std::string stringLiteral;
    while (position < input.size() && input[position] != '"') {
        stringLiteral += input[position++];
    }
    if (position >= input.size()) {
        throw std::runtime_error("Unterminated string literal");
    }
    position++;
    return { STRING_LITERAL, stringLiteral };
}

Token Lexer::getNextToken() {
    while (true) {
        skipWhitespace();
        skipComments();

        if (position >= input.size()) {
            return Token(END_OF_FILE, "");
        }

        char currentChar = input[position];
        switch (currentChar) {
            case '=': position++; return { EQUAL, "=" };
            case ':': position++; return { COLON, ":" };
            case ',': position++; return { COMMA, "," };
            case ';': position++; return { SEMICOLON, ";" };
            case '{': position++; return { LBRACE, "{" };
            case '}': position++; return { RBRACE, "}" };
            default:
                if (isalpha(currentChar)) {
                    return lexIdentifier();
                } else if (isdigit(currentChar)) {
                    return lexIntegerLiteral();
                } else {
                    throw std::runtime_error("Unexpected character: " + std::to_string(currentChar));
                }
        }
    }
}


