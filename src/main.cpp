#include "../include/sysCall.h"
#include <string>

int main() {
    Compiler compiler;
    std::string code = "int* = 5;";
    compiler.compile(code);
}
