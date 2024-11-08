#include "lexer.h"  
#include <iostream>
#include <fstream>
#include <filesystem>
#include "parser.h"

namespace fs = std::filesystem;

// void processFile(const std::string& filepath) {
//     std::ifstream file(filepath);
//     if (!file.is_open()) {
//         std::cerr << "Failed to open file: " << filepath << std::endl;
//         return;
//     }

//     // Read file contents into a string
//     std::string input((std::istreambuf_iterator<char>(file)),
//                       std::istreambuf_iterator<char>());

//     std::cout << "Processing file: " << filepath << std::endl;
//     Lexer lexer(input);
//     Parser parser(lexer);

//     try {
//         parser.parseProgram();
//         std::cout << "Parsed successfully: " << filepath << "\n" << std::endl;
//     } catch (const std::exception& e) {
//         std::cerr << "Error in file " << filepath << ": " << e.what() << "\n" << std::endl;
//     }
// }

// int main() {
//     std::string testDir = "tests";
    
//     if (!fs::exists(testDir) || !fs::is_directory(testDir)) {
//         std::cerr << "Test directory not found: " << testDir << std::endl;
//         return 1;
//     }

//     // Iterate over each file in the test directory
//     for (const auto& entry : fs::directory_iterator(testDir)) {
//         if (entry.is_regular_file()) {
//             processFile(entry.path().string());
//         }
//     }

//     return 0;
// }

// #include "lexer.h"  
// #include "parser.h"
// #include <iostream>
// #include <fstream>
// #include <filesystem>
// #include <vector>
// #include <string>

// namespace fs = std::filesystem;

// struct TestResult {
//     std::string filepath;
//     std::string result;  // "Passed" or error message
// };

// TestResult processFile(const std::string& filepath) {
//     std::ifstream file(filepath);
//     if (!file.is_open()) {
//         return {filepath, "Failed to open file"};
//     }

//     // Read file contents into a string
//     std::string input((std::istreambuf_iterator<char>(file)),
//                       std::istreambuf_iterator<char>());

//     Lexer lexer(input);
//     Parser parser(lexer);

//     try {
//         parser.parseProgram();
//         return {filepath, "Passed"};
//     } catch (const std::exception& e) {
//         return {filepath, e.what()};
//     }
// }

// int main() {
//     std::string testDir = "tests";
//     std::vector<TestResult> results;

//     if (!fs::exists(testDir) || !fs::is_directory(testDir)) {
//         std::cerr << "Test directory not found: " << testDir << std::endl;
//         return 1;
//     }

//     // Iterate over each file in the test directory
//     for (const auto& entry : fs::directory_iterator(testDir)) {
//         if (entry.is_regular_file()) {
//             TestResult result = processFile(entry.path().string());
//             results.push_back(result);

//             // Print result immediately for each file
//             std::cout << "Processing file: " << result.filepath << std::endl;
//             if (result.result == "Passed") {
//                 std::cout << "Parsed successfully: " << result.filepath << "\n" << std::endl;
//             } else {
//                 std::cerr << "Error in file " << result.filepath << ": " << result.result << "\n" << std::endl;
//             }
//         }
//     }

//     // Print summary of errors
//     std::cout << "\n--- Summary ---\n";
//     for (const auto& result : results) {
//         if (result.result == "Passed") {
//             std::cout << result.filepath << ": Passed" << std::endl;
//         } else {
//             std::cerr << result.filepath << ": Error - " << result.result << std::endl;
//         }
//     }

//     return 0;
// }

#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

struct TestResult {
    std::string filepath;
    std::string result;  // "Passed" or error message
};

TestResult processFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return {filepath, "Failed to open file"};
    }

    // Read file contents into a string
    std::string input((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());

    Lexer lexer(input);
    Parser parser(lexer);

    try {
        parser.parseProgram();
        return {filepath, "Passed"};
    } catch (const std::exception& e) {
        return {filepath, e.what()};
    }
}

int main() {
    std::string testDir = "tests";
    std::vector<TestResult> results;

    if (!fs::exists(testDir) || !fs::is_directory(testDir)) {
        std::cerr << "Test directory not found: " << testDir << std::endl;
        return 1;
    }

    // Iterate over each file in the test directory
    for (const auto& entry : fs::directory_iterator(testDir)) {
        if (entry.is_regular_file()) {
            TestResult result = processFile(entry.path().string());
            results.push_back(result);

            // Print result for the current file
            std::cout << "Processing file: " << result.filepath << std::endl;
            if (result.result == "Passed") {
                std::cout << "Parsed successfully: " << result.filepath << "\n" << std::endl;
            } else {
                std::cerr << "Error in file " << result.filepath << ": " << result.result << "\n" << std::endl;
            }

            // Wait for user input before proceeding to the next file
            std::cout << "Press Enter to continue to the next file..." << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    // Print summary of errors
    std::cout << "\n--- Summary ---\n";
    for (const auto& result : results) {
        if (result.result == "Passed") {
            std::cout << result.filepath << ": Passed" << std::endl;
        } else {
            std::cerr << result.filepath << ": Error - " << result.result << std::endl;
        }
    }

    return 0;
}


