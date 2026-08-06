#include "../include/BashGenerator.h"

void BashGenerator::load_ast(std::unique_ptr<ScopeBlock> ast__) {
    ast = std::move(ast__);
}
void BashGenerator::generate_bash() {
    ast->accept(*this);
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
void BashGenerator::visit(StringLiteral& node) {
    bash << "\\\"" << node.string << "\\\"";
}
void BashGenerator::visit(FloatLiteral& node) {
    bash << node.number;
}
void BashGenerator::visit(IntegerLiteral& node) {
    bash << node.number;
}
void BashGenerator::visit(BooleanLiteral& node) {
    bash << (node.state ? "true" : "false");
}
void BashGenerator::visit(VoidLiteral& node) {
    bash << "\\\"NONE\\\"";
}
void BashGenerator::visit(VariableDefinition& node) {
    bash << node.identifier << "=";
    node.value->accept(*this);
    bash << "\n";
}
void BashGenerator::visit(BinaryExpression& node) {
    bash << "$(echo \"";
    node.left->accept(*this);

    switch (node.op) {
        case BinaryOperation::ADD:
            bash << "+";
            break;
        case BinaryOperation::SUBTRACT:
            bash << "-";
            break;
        case BinaryOperation::MULTIPLY:
            bash << "*";
            break;
        case BinaryOperation::DIVIDE:
            bash << "/";
            break;
        default:
            break;
    }
    node.right->accept(*this);
    bash << "\" | bc -l)";
}
void BashGenerator::visit(IfStatement& node) {}
void BashGenerator::visit(FunctionCallExpr& node) {}
void BashGenerator::visit(VariableReference& node) {
    bash << "\"${" << node.identifier << "}\"";
}
void BashGenerator::visit(UnaryExpression& node) {}
void BashGenerator::visit(VariableReassignment& node) {}