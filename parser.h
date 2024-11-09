#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "symbolTable.h"

class Parser {
private:
    Lexer lexer;
    Token currentToken;
    SymbolTable symbolTable;
    std::string currentScope;

    void advance();
    void syntaxError();
    void parseGlobalVars();
    void parseScope();
    void parsePublicVars();
    void parsePrivateVars();
    void parseVarList(bool isPublic);
    void parseStmtList();
    // void parseStmt();

public:
    explicit Parser(const Lexer& lex);
    void parseProgram();
    void printResolutons() {symbolTable.printResolutions();}
};

#endif
