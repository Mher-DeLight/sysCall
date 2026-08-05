#include "../include/sysCall.h"
#include <string>

int main() {
    Compiler compiler;
    std::string code = "string a = \"hello\";";
    compiler.compile(code);
}
