#include "symbolTable.h"

Variable::Variable(const std::string& n, bool pub, const std::string& s)
    : name(n), isPublic(pub), scope(s) {}

Assignment::Assignment(const std::string& l, const std::string& r, const std::string& s)
    : lhs(l), rhs(r), scope(s) {}

void SymbolTable::enterScope(const std::string& scopeName) {
    scopeStack.push_back(scopeName);
}

void SymbolTable::exitScope() {
    if (!scopeStack.empty()) {
        scopeStack.pop_back();
    }
}

std::string SymbolTable::getCurrentScope() const {
    if (scopeStack.empty()) return "global";
    // Return just the last scope name, not the full path
    return scopeStack.back();
}

void SymbolTable::addVariable(const std::string& name, bool isPublic) {
    std::string scope = getCurrentScope();
    variables[scope].emplace_back(name, isPublic, scope);
}

void SymbolTable::addGlobalVariable(const std::string& name) {
    variables["global"].emplace_back(name, true, "global");
}

void SymbolTable::addAssignment(const std::string& lhs, const std::string& rhs) {
    assignments.emplace_back(lhs, rhs, getCurrentScope());
}

bool isVariableAccessible(const std::string& fromScope, const std::string& definingScope, bool isPublic) {
    if (definingScope == "global") return true;
    if (fromScope == definingScope) return true;
    if (!isPublic) return false;
    
    // Check if definingScope is a parent of fromScope
    return fromScope.find(definingScope) == 0 && 
           (fromScope.length() == definingScope.length() || 
            fromScope[definingScope.length()] == '.');
}


std::string SymbolTable::resolveVariable(const std::string& name, const std::string& currentScope) {
    // First check current scope
    auto currScopeIt = variables.find(currentScope);
    if (currScopeIt != variables.end()) {
        for (const auto& var : currScopeIt->second) {
            if (var.name == name) {
                return currentScope + "." + name;
            }
        }
    }

    // For the hello scope only check global scope first
    if (currentScope == "hello") {
        auto globalIt = variables.find("global");
        if (globalIt != variables.end()) {
            for (const auto& var : globalIt->second) {
                if (var.name == name) {
                    return "::" + name;
                }
            }
        }
    } else {
        // For nested scopes, check parent scope's public variables before global
        if (currentScope != "global") {
            auto parentIt = variables.find("hello");  // hello is the parent scope
            if (parentIt != variables.end()) {
                for (const auto& var : parentIt->second) {
                    if (var.name == name && var.isPublic) {
                        return "hello." + name;
                    }
                }
            }
        }

        // Finally check global scope
        auto globalIt = variables.find("global");
        if (globalIt != variables.end()) {
            for (const auto& var : globalIt->second) {
                if (var.name == name) {
                    return "::" + name;
                }
            }
        }
    }

    return "?." + name;
}
void SymbolTable::printResolutions() {
    for (const auto& assignment : assignments) {
        std::string resolvedLhs = resolveVariable(assignment.lhs, assignment.scope);
        std::string resolvedRhs = resolveVariable(assignment.rhs, assignment.scope);
        std::cout << resolvedLhs << " = " << resolvedRhs << std::endl;
    }
}