#pragma once
#include "Tokenizer.h"
#include <string>
#include <vector>

class Compiler {
private:
    Tokenizer tokenizer;

public:
    void compile(const std::string& code);
    std::vector<std::string> get_bash();
};
