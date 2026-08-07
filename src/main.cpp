#include "../include/sysCall.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        throw std::runtime_error("No files provided");
    }
    std::ifstream fileStream(argv[1]);
    if (!fileStream.is_open()) {
        std::string message = "couldn't open file:";
        message.append(argv[1]);
        throw std::runtime_error(message);
    }
    std::ostringstream sstr;
    sstr << fileStream.rdbuf();

    std::ofstream output("../output.sh");
    if (!output.is_open()) {
        throw std::runtime_error("couldn't open output file lolze");
    }

    Compiler compiler;
    // compiler.compile(sstr.str(), output);

    // output.close();

    // int result = system("bash ../output.sh");
    // return result;

    compiler.compile(sstr.str(), std::cout);
    std::cout << std::endl;
}
