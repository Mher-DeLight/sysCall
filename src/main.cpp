#include "../include/sysCall.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

int main() {
    std::ifstream fileStream("code.scl");
    if (!fileStream.is_open()) {
        throw std::runtime_error("couldn't open file lolz");
    }
    std::ostringstream sstr;
    sstr << fileStream.rdbuf();

    Compiler compiler;
    compiler.compile(sstr.str());
}
