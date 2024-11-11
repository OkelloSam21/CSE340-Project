#include "symbolTable.h"

Variable::Variable(const std::string& n, bool pub, const std::string& s)
    : name(n), isPublic(pub), scope(s) {}

Assignment::Assignment(const std::string& l, const std::string& r, const std::string& s)
    : lhs(l), rhs(r), scope(s) {}

void SymbolTable::enterScope(const std::string& scopeName) {
    std::string newScope;
    if (scopeStack.empty()) {
        newScope = scopeName;
    } else {
        newScope = scopeStack.back() + "." + scopeName;
    }
    scopeStack.push_back(newScope);
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
    // 1. First check current scope
    auto currScopeIt = variables.find(currentScope);
    if (currScopeIt != variables.end()) {
        for (const auto& var : currScopeIt->second) {
            if (var.name == name) {
                return currentScope + "." + name;
            }
        }
    }

    // 2. Check parent scopes for public variables
    size_t lastDot = currentScope.rfind('.');
    while (lastDot != std::string::npos) {
        std::string parentScope = currentScope.substr(0, lastDot);
        auto parentVarsIt = variables.find(parentScope);
        if (parentVarsIt != variables.end()) {
            for (const auto& var : parentVarsIt->second) {
                if (var.name == name && var.isPublic) {
                    return parentScope + "." + name;
                }
            }
        }
        lastDot = parentScope.rfind('.');
    }

    // 3. Check the top-level scope (if we're in a nested scope)
    if (currentScope.find('.') != std::string::npos) {
        std::string topScope = currentScope.substr(0, currentScope.find('.'));
        auto topVarsIt = variables.find(topScope);
        if (topVarsIt != variables.end()) {
            for (const auto& var : topVarsIt->second) {
                if (var.name == name && var.isPublic) {
                    return topScope + "." + name;
                }
            }
        }
    }

    // 4. Check global scope
    auto globalIt = variables.find("global");
    if (globalIt != variables.end()) {
        for (const auto& var : globalIt->second) {
            if (var.name == name) {
                return "::" + name;
            }
        }
    }

    // 5. Variable not found in any accessible scope
    return "?." + name;
}

void SymbolTable::printResolutions() {
    for (const auto& assignment : assignments) {
        std::string resolvedLhs = resolveVariable(assignment.lhs, assignment.scope);
        std::string resolvedRhs = resolveVariable(assignment.rhs, assignment.scope);
        std::cout << resolvedLhs << " = " << resolvedRhs << std::endl;
    }
}