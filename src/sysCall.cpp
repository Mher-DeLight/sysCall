#include "../include/sysCall.h"
#include <iostream>
#include <string>

void Compiler::compile(const std::string& code) {
    tokenizer.tokenize(code);
    tokenizer.pretty_print(std::cout);

    parser.load_tokens(std::move(tokenizer.get_tokens()));
    parser.parse();

    sema.load_ast(parser.hand_over_AST());
    sema.analyse();

    std::cout << "\n\n== FINAL BASH OUTPUT ==" << std::endl;
    gen.load_ast(sema.hand_over_AST());
    gen.generate_bash();
    std::cout << gen.get_bash();
}
