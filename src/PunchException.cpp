#include "PunchException.h"

#include <iostream>

void PunchException::handleException(const PunchException& e) {
    if (dynamic_cast<const ScannerException*>(&e) != nullptr) {
        std::cout << "Scanner error: " << e.getMessage();
    } else if (dynamic_cast<const ParserException*>(&e) != nullptr) {
        std::cout << "Parser error: " << e.getMessage();
    }
    std::cout << std::endl;
}
