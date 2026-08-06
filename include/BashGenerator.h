#pragma once
#include "Common.h"
#include <string>

class BashGenerator : public Visitor {
private:
    std::vector<std::string> bash;
    std::unique_ptr<ScopeBlock> ast;

public:
    void load_ast(std::unique_ptr<ScopeBlock> ast__);

    void visit(ScopeBlock& node) override;
    void visit(StringLiteral& node) override;
    void visit(FloatLiteral& node) override;
    void visit(IntegerLiteral& node) override;
    void visit(BooleanLiteral& node) override;
    void visit(VoidLiteral& node) override;
    void visit(VariableDefinition& node) override;
    void visit(BinaryExpression& node) override;
    void visit(IfStatement& node) override;
    void visit(FunctionCallExpr& node) override;
    void visit(VariableReference& node) override;
    void visit(UnaryExpression& node) override;
    void visit(VariableReassignment& node) override;

    void generate_bash();
    std::vector<std::string> get_bash();
};