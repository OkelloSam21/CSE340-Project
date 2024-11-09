#include <vector>
#include <iostream>
#include "parser.h"


Parser::Parser(const Lexer &lex) : lexer(lex)
{
    advance();
    std::cout << "token: " << currentToken.value << " (" << currentToken.type << ")" << std::endl;
}

void Parser::advance()
{
    currentToken = lexer.getNextToken();
}

void Parser::syntaxError()
{
    std::cerr << "Syntax Error";
    exit(1);
}

void Parser::parseProgram()
{
    parseGlobalVars();
    parseScope();
}

void Parser::parseGlobalVars() {
    if (currentToken.type != ID) {
        syntaxError();
    }

    while (currentToken.type == ID) {
        // Process the current identifier (e.g., add it to the symbol table)
        std::string identifier = currentToken.value;
        symbolTable.addVariable(identifier, /* isPublic */ true);

        advance(); // Move to the next token

        if (currentToken.type == COMMA) {
            advance(); // Move past the comma
            if (currentToken.type != ID) {
                syntaxError();
            }
        } else if (currentToken.type == SEMICOLON) {
            advance(); // Move past the semicolon
            break;
        } else {
            syntaxError();
        }
    }
}

void Parser::parseScope()
{
    // Expect an ID
    if (currentToken.type != ID) {
        syntaxError();
    }
    std::string scopeName = currentToken.value;  // Store scope name for symbol table
    advance();

    // Expect a left brace
    if (currentToken.type != LBRACE) {
        syntaxError();
    }
    advance();

    // Handle optional public and private vars
    if (currentToken.type == PUBLIC) {
        parsePublicVars();
    }
    if (currentToken.type == PRIVATE) {
        parsePrivateVars();
    }

    // Parse statement list
    parseStmtList();

    // Expect a right brace
    if (currentToken.type != RBRACE) {
        syntaxError();
    }
    advance();
}

void Parser::parsePublicVars()
{
    // We're already on PUBLIC token
    advance();  // Move past PUBLIC
    
    if (currentToken.type != COLON) {
        syntaxError();
    }
    advance();  // Move past COLON
    
    parseVarList();
}

void Parser::parsePrivateVars()
{
    // We're already on PRIVATE token
    advance();  // Move past PRIVATE
    
    if (currentToken.type != COLON) {
        syntaxError();
    }
    advance();  // Move past COLON
    
    parseVarList();
}

void Parser::parseVarList()
{
    if (currentToken.type != ID) {
        syntaxError();
    }

    do {
        advance();  // Move past ID or COMMA
        
        if (currentToken.type == SEMICOLON) {
            advance();  // Move past semicolon
            break;
        } else if (currentToken.type != COMMA) {
            syntaxError();
        }
    } while (true);
}

// void Parser::parsePublicVars()
// {
//     // if (currentToken.type == PUBLIC) {
//         advance();
//         if (currentToken.type == COLON) {
//             advance();
//             parseVarList();
//             if (currentToken.type == SEMICOLON) {
//                 advance();
//             } else {
//                 syntaxError();
//             }
//         } else {
//             syntaxError();
//         }
//     // }
// }

// void Parser::parsePrivateVars()
// {
//     // if (currentToken.type == PRIVATE) {
//         advance();
//         if (currentToken.type == COLON) {
//             advance();
//             parseVarList();
//             if (currentToken.type == SEMICOLON) {
//                 advance();
//             } else {
//                 syntaxError();
//             }
//         } else {
//             syntaxError();
//         }
//     // }
// }

// void Parser::parseVarList()
// {
//     if (currentToken.type != ID) {
//         syntaxError();  // Expecting an ID
//     }

//     while (true) {
//         advance();

//         if (currentToken.type == COMMA) {
//             advance();
//             if (currentToken.type != ID) {
//                 syntaxError();  // Comma must be followed by an ID
//             }
//         } else if (currentToken.type == SEMICOLON) {
//             advance();
//             break;
//         } else {
//             syntaxError();  // Expecting either a comma or a semicolon
//         }
//     }
// }

void Parser::parseStmtList()
{
    while (currentToken.type != RBRACE && currentToken.type != END_OF_FILE) {
        parseStmt();
    }
}

void Parser::parseStmt()
{
    if (currentToken.type == ID) {
        advance();
        if (currentToken.type == EQUAL) {
            advance();
            if (currentToken.type == ID) {
                advance();
                if (currentToken.type == SEMICOLON) {
                    advance();
                } else {
                    syntaxError();
                }
            } else {
                syntaxError();
            }
        } else if (currentToken.type == LBRACE) {
            parseScope();
        } else {
            syntaxError();
        }
    } else {
        syntaxError();
    }
}
