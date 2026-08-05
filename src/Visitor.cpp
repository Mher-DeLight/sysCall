#include "../include/Common.h"
#include <iostream>

void PrettyPrinter::printIndent() {
    for (int i = 0; i < indent; i++) {
        stream << "    ";
    }
}
void PrettyPrinter::visit(EntryPoint& node) {
    printIndent();
    stream << "== ENTRY POINT ==" << std::endl;

    indent++;
    for (auto& child : node.children) {
        child->accept(*this);
    }
    indent--;
}
void PrettyPrinter::visit(VariableDefinition& node) {
    printIndent();
    stream << "VariableDefinition\n";

    indent++;
    printIndent();
    stream << "Identifier(" << node.identifier << ")\n";
    printIndent();
    stream << "Type(" << (int)node.type << ")\n";
    node.value->accept(*this);
    indent--;
}
void PrettyPrinter::visit(IntegerLiteral& node) {
    printIndent();
    stream << "IntegerLiteral(" << node.number << ")\n";
}
void PrettyPrinter::visit(FloatLiteral& node) {
    printIndent();
    stream << "FloatLiteral(" << node.number << ")\n";
}
void PrettyPrinter::visit(StringLiteral& node) {
    printIndent();
    stream << "StringLiteral(\"" << node.string << "\")\n";
}
void PrettyPrinter::visit(BooleanLiteral& node) {
    printIndent();
    stream << "BooleanLiteral(" << (node.state ? "true)\n" : "false)\n");
}
void PrettyPrinter::visit(VoidLiteral& node) {
    printIndent();
    stream << "VoidLiteral";
}
