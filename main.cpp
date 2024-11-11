#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <fstream>
#include <string>

// Function to process a single file
void processFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return;
    }

    std::string input((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());

    try {
        Lexer lexer(input);
        Parser parser(lexer);
        parser.parseProgram();
        parser.printResolutons();
    } catch (...) {
        std::cout << "Syntax Error\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filepath = argv[1];
    processFile(filepath);

    return 0;
}
