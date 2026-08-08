#pragma once
#include "BashGenerator.h"

class BashPrinter {
    std::vector<std::unique_ptr<BashStatement>> bash;

    void print(BashBinaryExpression& node, std::ostream& stream);
    void print(BashLiteral& node, std::ostream& stream);
    void print(BashFunctionCallExpression& node, std::ostream& stream);
    void print(BashVariableReference& node, std::ostream& stream);

    void print(BashAssignment& node, std::ostream& stream);
    void print(BashIfStatement& node, std::ostream& stream);
    void print(BashEndIfStatement& node, std::ostream& stream);
    void print(BashFunctionCallStatement& node, std::ostream& stream);
    void print(BashElseIfStatement& node, std::ostream& stream);
    void print(BashElseStatement& node, std::ostream& stream);

    void print_dispatcher(Bash* node, std::ostream& stream);
    std::string wrap(const std::string& string, WrapType wrap_type);
    std::string get_operation(BinaryOperation operation);

public:
    void load_bash(std::vector<std::unique_ptr<BashStatement>> bash_);
    void print_bash(std::ostream& stream);
};