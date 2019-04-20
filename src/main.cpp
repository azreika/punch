#include "Scanner.h"

#include <fstream>
#include <iostream>
#include <sstream>

void printUsage() { std::cout << "Usage: punch <FILE>" << std::endl; }

void compileProgram(std::string filename, std::ostream& out) {
    // read in the source code
    std::stringstream source;
    std::ifstream file(filename);
    source << file.rdbuf();

    // run the scanner
    out << " --- scanner --- " << std::endl;
    Scanner scanner(source.str());
    for (auto tok : scanner.getTokens()) {
        out << tok << std::endl;
    }
    out << std::endl;

    // run the parser
    out << " --- parser --- " << std::endl;
    out << "TODO..." << std::endl;
    out << std::endl;

    // write result to out
    out << " --- result --- " << std::endl;
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

    std::string outputFilename = "./out/out.sh";
    std::ofstream outputFile(outputFilename);
    outputFile << result.str();
    std::cout << result.str();

    return 0;
}
