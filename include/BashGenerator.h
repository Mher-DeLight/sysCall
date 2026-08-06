#pragma once
#include "Common.h"
#include "ErrorHandler.h"
#include <sstream>
#include <string>

class BashGenerator : public Visitor {
private:
    void genPanic(const std::string& msg, const SourceLocation& src = SourceLocation());
    std::stringstream bash;

    std::unique_ptr<ScopeBlock> ast;
    ExpressionInfo analyseExpression(Expression* expr);
    void numberOperation(BinaryExpression& node);
    void stringOperation(BinaryExpression& node);

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
    void visit(FunctionCallStmt& node) override;

    void generate_bash();
    std::string get_bash();
};