#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

class Parser {
private:
    Lexer lexer;
    Token currentToken;

    void advance();
    void syntaxError();

    void parseGlobalVars();
    void parseScope();
    void parsePublicVars();
    void parsePrivateVars();
    void parseVarList();
    void parseStmtList();
    void parseStmt();

public:
    explicit Parser(const Lexer& lex);
    void parseProgram();
};

#endif // PARSER_H
