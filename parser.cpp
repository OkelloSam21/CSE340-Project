#include <iostream>
#include "parser.h"

Parser::Parser(const Lexer &lex) : lexer(lex), currentScope("global")
{
    advance();
}

void Parser::advance()
{
    currentToken = lexer.getNextToken();
}

void Parser::syntaxError()
{
    std::cout << "Syntax Error";
    exit(1);
}

void Parser::parseProgram() {
    parseGlobalVars();

    if (currentToken.type == ID) {
        parseScope();
    }

    if (currentToken.type != END_OF_FILE) {
        syntaxError();
    }
}

void Parser::parseScope() {
    if (currentToken.type != ID) {
        syntaxError();
    }
    
    



    std::string scopeName = currentToken.value;
    symbolTable.enterScope(scopeName);
    advance();

    if (currentToken.type != LBRACE) {
        syntaxError();
    }
    advance();

    // Handle optional public/private sections
    if (currentToken.type == PUBLIC) {
        parsePublicVars();
    }
    if (currentToken.type == PRIVATE) {
        parsePrivateVars();
    }

    parseStmtList();

    if (currentToken.type != RBRACE) {
        syntaxError();
    }
    symbolTable.exitScope();
    advance();
}

void Parser::parseGlobalVars()
{
    if (currentToken.type != ID)
    {
        return;
    }

    do
    {
        symbolTable.addGlobalVariable(currentToken.value);
        advance();

        if (currentToken.type == SEMICOLON)
        {
            advance();
            break;
        }
        else if (currentToken.type != COMMA)
        {
            syntaxError();
        }
        advance();

        if (currentToken.type != ID)
        {
            syntaxError();
        }
    } while (true);
}

void Parser::parsePublicVars()
{
    advance();

    if (currentToken.type != COLON)
    {
        syntaxError();
    }
    advance();

    parseVarList(true);
}

void Parser::parsePrivateVars()
{
    advance();
    if (currentToken.type != COLON)
    {
        syntaxError();
    }
    advance();

    parseVarList(false);
}

void Parser::parseStmtList() {   
    while (currentToken.type != RBRACE && currentToken.type != END_OF_FILE) {
        if (currentToken.type != ID) {
            syntaxError();
        }
        
        std::string id = currentToken.value;
        advance();
        
        if (currentToken.type == EQUAL) {
            advance();
            
            if (currentToken.type != ID) {
                syntaxError();
            }
            
            std::string rhs = currentToken.value;
            symbolTable.addAssignment(id, rhs);
            advance();
            
            if (currentToken.type != SEMICOLON) {
                syntaxError();
            }
            advance();
        } else if (currentToken.type == LBRACE) {
            advance();
            
            symbolTable.enterScope(id);

            if (currentToken.type == PUBLIC) {
                parsePublicVars();
            }
            if (currentToken.type == PRIVATE) {
                parsePrivateVars();
            }
            
            parseStmtList();
            
            if (currentToken.type != RBRACE) {
                syntaxError();
            }
            
            symbolTable.exitScope();
            advance();
        } else {
            syntaxError();
        }
    }
}

void Parser::parseVarList(bool isPublic)
{
    if (currentToken.type != ID)
    {
        syntaxError();
    }

    do
    {
        symbolTable.addVariable(currentToken.value, isPublic);
        advance();

        if (currentToken.type == SEMICOLON)
        {
            advance();
            break;
        }

        if (currentToken.type != COMMA)
        {
            syntaxError();
        }

        advance();

        if (currentToken.type != ID)
        {
            syntaxError();
        }
    } while (true);
}
