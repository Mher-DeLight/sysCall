#include "../include/SysCAll.h"
#include <iostream>
#include <string>

void Compiler::compile(const std::string& code) {
    tokenizer.tokenize(code);
    tokenizer.pretty_print(std::cout);
}
