#pragma once
#include "BashGenerator.h"
#include "Parser.h"
#include "SemanticAnalyser.h"
#include "Tokenizer.h"

#include <string>
#include <vector>

class Compiler {
private:
    Tokenizer tokenizer;
    Parser parser;
    SemanticAnalyser sema;
    BashGenerator gen;

public:
    void compile(const std::string& code);
    std::vector<std::string> get_bash();
};
