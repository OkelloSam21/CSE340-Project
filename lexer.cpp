#include "lexer.h"
#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string& src) : input(src) {}

void Lexer::skipWhitespace() {
    while (position < input.size() && isspace(input[position])) {
        position++;
    }
}

void Lexer::skipComments() {
    while (position < input.size()) {
        skipWhitespace();
        
        if (position + 1 < input.size() && 
            input[position] == '/' && 
            input[position + 1] == '/') {
            position += 2;  

            while (position < input.size() && input[position] != '\n') {
                position++;
            }
            if (position < input.size()) {
                position++;
            }
            
            skipWhitespace();
        } else {
            break;
        }
    }
}

Token Lexer::lexIdentifier() {
    std::string identifier;

    if (!isalpha(input[position])) {
        position++;
        return { ERROR, "" };
    }
    
    while (position < input.size() && (isalnum(input[position]) || input[position] == '_')) {
        identifier += input[position++];
    }

    if (identifier == "public") {
        return { PUBLIC, identifier };
    } else if (identifier == "private") {
        return { PRIVATE, identifier };
    } else {
        return { ID, identifier };
    }
}

Token Lexer::getNextToken() {
    while (position < input.size()) {
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
                } else {
                    // Return ERROR token instead of throwing exception
                    return { ERROR, std::string(1, currentChar) };
                }
        }
    }
    
    return Token(END_OF_FILE, "");
}

