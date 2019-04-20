#include <iostream>
#include <fstream>
#include <sstream>

void printUsage() {
    std::cout << "Usage: punch <FILE>" << std::endl;
}

void compileProgram(std::string filename, std::ostream& out) {
    out << "#!/bin/bash" << std::endl;
    out << std::endl;
    out << "echo this is the result of " << filename << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printUsage();
        return 1;
    }

    std::stringstream result;
    compileProgram(argv[1], result);

    std::string outputFilename = "out.sh";
    std::ofstream outputFile(outputFilename);
    outputFile << result.str();
    std::cout << result.str();

    return 0;
}
