#pragma once
#include "BashGenerator.h"
#include "BashPrinter.h"
#include "Parser.h"
#include "SemanticAnalyser.h"
#include "Tokenizer.h"

#include <iostream>
#include <string>
#include <vector>

class Compiler {
private:
    Tokenizer tokenizer;
    Parser parser;
    SemanticAnalyser sema;
    BashIrGenerator irGen;
    BashPrinter printer;

public:
    void compile(const std::string& code, std::ostream& stream = std::cout);
    std::vector<std::string> get_bash();
};
