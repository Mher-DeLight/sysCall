#include "../include/SysCAll.h"
#include <string>

int main() {
    Compiler compiler;
    std::string code = "int a3 .= 53.6";
    compiler.compile(code);
}
