// symbolTable.h
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

struct Variable {
    std::string name;
    bool isPublic;
    std::string scope;
    
    Variable(const std::string& n, bool pub, const std::string& s);
};

struct Assignment {
    std::string lhs;
    std::string rhs;
    std::string scope;
    
    Assignment(const std::string& l, const std::string& r, const std::string& s);
};

class SymbolTable {
private:
    std::vector<std::string> scopeStack;
    std::unordered_map<std::string, std::vector<Variable>> variables;
    std::vector<Assignment> assignments;

public:
    void enterScope(const std::string& scopeName);
    void exitScope();
    std::string getCurrentScope() const;
    void addVariable(const std::string& name, bool isPublic);
    void addGlobalVariable(const std::string& name);
    void addAssignment(const std::string& lhs, const std::string& rhs);
    std::string resolveVariable(const std::string& name, const std::string& currentScope);
    void printResolutions();
};

#endif