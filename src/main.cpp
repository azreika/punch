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

    // first argument: input filename
    std::string inFilename = argv[1];

    // second argument: (optional) output filename
    std::string outFilename = (argc == 3) ? argv[2] : "a.out.sh";

    // compile the program
    std::stringstream result;
    compileProgram(inFilename, result);

    // write the program to the output file and stdout
    std::ofstream outFile(outFilename);
    outFile << result.str();
    std::cout << result.str();

    return 0;
}
