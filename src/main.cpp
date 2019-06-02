#include "Parser.h"
#include "Scanner.h"
#include "Translator.h"

#include <fstream>
#include <iostream>
#include <sstream>

void printUsage() { std::cout << "Usage: punch INFILE [OUTFILE]" << std::endl; }

void compileProgram(std::string filename, std::ostream& out) {
    // read in the source code
    std::stringstream source;
    std::ifstream file(filename);
    source << file.rdbuf();

    // run the scanner
    Scanner scanner(source.str());

    // run the parser
    Parser parser(scanner.getTokens());
    AstProgram* program = parser.parse();

    // translate and write result to out
    Translator translator(out, program);
    translator.run();
}

int main(int argc, char** argv) {
    // expecting strictly 1 or 2 arguments
    if (argc != 2 && argc != 3) {
        printUsage();
        return 1;
    }

    // compile the program
    std::string inFilename = argv[1];
    std::stringstream result;
    compileProgram(inFilename, result);

    // decide where to write the result
    if (argc == 3) {
        // write to file
        std::string outFilename = argv[2];
        std::ofstream outFile(outFilename);
        outFile << result.str();
    } else {
        // write to stdout
        std::cout << result.str();
    }

    return 0;
}
