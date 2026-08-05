#pragma once
#include "Parser.h"
#include "Tokenizer.h"
#include <string>
#include <vector>

class Compiler {
private:
    Tokenizer tokenizer;
    Parser parser;

public:
    void compile(const std::string& code);
    std::vector<std::string> get_bash();
};
