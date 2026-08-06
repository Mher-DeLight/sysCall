#include "../include/BashGenerator.h"
#include <algorithm>
#include <cassert>
#include <iostream>

void BashGenerator::load_ast(std::unique_ptr<ScopeBlock> ast__) {
    ast = std::move(ast__);
}
void BashGenerator::generate_bash() {
    ast->accept(*this);
}
std::string BashGenerator::get_bash() {
    return bash.str();
}
void BashGenerator::genPanic(const std::string& msg, const SourceLocation& src) {
    if (src.row == -1) {
        panic("[BASH GENERATOR PANIC] " + msg);
    } else {
        panic("[BASH GENERATOR PANIC] " + msg + " [" + std::to_string(src.row) + ":" +
              std::to_string(src.column) + "]");
    }
}
void BashGenerator::numberOperation(BinaryExpression& node) {
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
void BashGenerator::stringOperation(BinaryExpression& node) {
    if (node.op != BinaryOperation::ADD)
        genPanic("invalid string operation", node.location);

    bash << "\"";
    node.left->accept(*this);
    node.right->accept(*this);
    bash << "\"";
}
// == VISIT ==
void BashGenerator::visit(ScopeBlock& node) {
    for (auto& child : node.children) {
        child->accept(*this);
    }
}
void BashGenerator::visit(StringLiteral& node) {
    bash << node.string;
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
    if (node.type == VariableType::STRING)
        bash << "\"";
    node.value->accept(*this);
    if (node.type == VariableType::STRING)
        bash << "\"";
    bash << "\n";
}
void BashGenerator::visit(BinaryExpression& node) {
    if (node.return_type == VariableType::INT || node.return_type == VariableType::FLOAT) {
        numberOperation(node);
    } else if (node.return_type == VariableType::STRING || node.return_type == VariableType::CHAR) {
        stringOperation(node);
    } else {
        genPanic("invalid binary expression", node.location);
    }
}
void BashGenerator::visit(IfStatement& node) {}
void BashGenerator::visit(FunctionCallExpr& node) {
    bash << "$(";

    size_t pos = node.identifier.find('.');
    std::string first =
        (pos == std::string::npos) ? node.identifier : node.identifier.substr(0, pos);

    if (std::find(includes.begin(), includes.end(), first) != includes.end()) {
        std::replace(node.identifier.begin(), node.identifier.end(), '.', ' ');
        bash << node.identifier << " ";
        node.args.at(0).get()->accept(*this);
        bash << ")";
    } else {
        genPanic("invalid to invoke " + node.identifier + " as expression");
    }
}
void BashGenerator::visit(VariableReference& node) {
    switch (node.return_type) {
        case VariableType::STRING:
        case VariableType::CHAR:
            bash << "${" << node.identifier << "}";
            break;

        case VariableType::INT:
        case VariableType::FLOAT:
            bash << "${" << node.identifier << "}";
            break;

        default:
            genPanic("unsupported variable type", node.location);
    }
}
void BashGenerator::visit(UnaryExpression& node) {}
void BashGenerator::visit(VariableReassignment& node) {}
void BashGenerator::visit(FunctionCallStmt& node) {
    size_t pos = node.identifier.find('.');
    std::string first =
        (pos == std::string::npos) ? node.identifier : node.identifier.substr(0, pos);

    if (std::find(includes.begin(), includes.end(), first) != includes.end()) {
        std::replace(node.identifier.begin(), node.identifier.end(), '.', ' ');
        bash << node.identifier << " ";
        node.args.at(0).get()->accept(*this);
        bash << "\n";
    }
}