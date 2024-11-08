#include <unordered_map>
#include <string>

struct Variable {
    std::string name;
    bool isPublic;
};

class SymbolTable {
private:
    std::unordered_map<std::string, Variable> variables;

public:
    void addVariable(const std::string& name, bool isPublic) {
        variables[name] = { name, isPublic };
    }

    Variable* lookup(const std::string& name) {
        if (variables.find(name) != variables.end()) {
            return &variables[name];
        }
        return nullptr;
    }
};
