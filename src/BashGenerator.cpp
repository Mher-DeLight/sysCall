#include "../include/BashGenerator.h"

void BashGenerator::load_ast(std::unique_ptr<ScopeBlock> ast__) {
    ast = std::move(ast__);
}
void BashGenerator::generate_bash() {
    bash << "echo hello\n";
}
std::string BashGenerator::get_bash() {
    return bash.str();
}

// == VISIT ==
void BashGenerator::visit(ScopeBlock& node) {
    for (auto& child : node.children) {
        child->accept(*this);
    }
}
void BashGenerator::visit(StringLiteral& node) {}
void BashGenerator::visit(FloatLiteral& node) {}
void BashGenerator::visit(IntegerLiteral& node) {}
void BashGenerator::visit(BooleanLiteral& node) {}
void BashGenerator::visit(VoidLiteral& node) {}
void BashGenerator::visit(VariableDefinition& node) {}
void BashGenerator::visit(BinaryExpression& node) {}
void BashGenerator::visit(IfStatement& node) {}
void BashGenerator::visit(FunctionCallExpr& node) {}
void BashGenerator::visit(VariableReference& node) {}
void BashGenerator::visit(UnaryExpression& node) {}
void BashGenerator::visit(VariableReassignment& node) {}