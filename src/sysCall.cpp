#include "../include/sysCall.h"
#include <iostream>
#include <string>

void Compiler::compile(const std::string& code) {
    tokenizer.tokenize(code);
    tokenizer.pretty_print(std::cout);

    parser.load_tokens(std::move(tokenizer.get_tokens()));
    parser.parse();
}
