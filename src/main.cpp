#include "../include/SysCAll.h"
#include <string>

int main() {
    Compiler compiler;
    std::string code = "int&";
    compiler.compile(code);
}
