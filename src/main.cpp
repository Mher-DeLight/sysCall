#include "../include/SysCAll.h"
#include <string>

int main() {
    Compiler compiler;
    std::string code = "int a3 .= 5";
    compiler.compile(code);
}
