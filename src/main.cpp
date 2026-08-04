#include "../include/sysCall.h"
#include <string>

int main() {
    Compiler compiler;
    std::string code = " \
        int* a = 'k'; \
        float b = 3.2; \
    ";
    compiler.compile(code);
}
