#include "../include/SysCAll.h"
#include <string>

int main() {
    Compiler compiler;
    std::string code = "int a .= 5";
    compiler.compile(code);
}
