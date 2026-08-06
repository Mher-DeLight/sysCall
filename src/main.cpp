#include "../include/sysCall.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

int main() {
    std::ifstream fileStream("../code.scl");
    if (!fileStream.is_open()) {
        throw std::runtime_error("couldn't open file lolz");
    }
    std::ostringstream sstr;
    sstr << fileStream.rdbuf();

    std::ofstream output("../output.sh");
    if (!output.is_open()) {
        throw std::runtime_error("couldn't open output file lolze");
    }

    Compiler compiler;
    compiler.compile(sstr.str(), output);

    output.close();
}
