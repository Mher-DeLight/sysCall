#include "../include/BashPrinter.h"
#include "../include/Common.h"
#include "../include/ErrorHandler.h"
#include <algorithm>
#include <iostream>

void BashPrinter::load_bash(std::vector<std::unique_ptr<BashStatement>> bsh) {
    bash = std::move(bsh);
}
void BashPrinter::print_dispatcher(Bash* node, std::ostream& stream) {
    if (auto nd = dynamic_cast<BashAssignment*>(node)) {
        return print(*nd, stream);
    } else if (auto nd = dynamic_cast<BashIfStatement*>(node)) {
        return print(*nd, stream);
    } else if (auto nd = dynamic_cast<BashEndIfStatement*>(node)) {
        return print(*nd, stream);
    } else if (auto nd = dynamic_cast<BashFunctionCallStatement*>(node)) {
        return print(*nd, stream);
    } else if (auto nd = dynamic_cast<BashBinaryExpression*>(node)) {
        return print(*nd, stream);
    } else if (auto nd = dynamic_cast<BashLiteral*>(node)) {
        return print(*nd, stream);
    } else if (auto nd = dynamic_cast<BashFunctionCallExpression*>(node)) {
        return print(*nd, stream);
    } else if (auto nd = dynamic_cast<BashVariableReference*>(node)) {
        return print(*nd, stream);
    }
}
std::string BashPrinter::wrap(const std::string& string, WrapType wrap_type) {
    if (wrap_type == WrapType::NONE) {
        return string;
    }
    if (wrap_type == WrapType::DOUBLE_QUOTE) {
        return "\"" + string + "\"";
    }
    if (wrap_type == WrapType::SINGLE_QUOTE) {
        return "\'" + string + "\'";
    }
    if (wrap_type == WrapType::VARIABLE_WRAP) {
        return "${" + string + "}";
    }
    if (wrap_type == WrapType::SINGLE_VARIABE_WRAP) {
        return "\'${" + string + "}\'";
    }
    if (wrap_type == WrapType::DOUBLE_VARIABLE_WRAP) {
        return "\"${" + string + "}\"";
    }
    if (wrap_type == WrapType::COMMAND_WRAP) {
        return "$(" + string + ")";
    }
    if (wrap_type == WrapType::SINGLE_COMMAND_WRAP) {
        return "\'$(" + string + ")\'";
    }
    if (wrap_type == WrapType::DOUBLE_COMMAND_WRAP) {
        return "\"$(" + string + ")\"";
    }
}
std::string BashPrinter::get_operation(BinaryOperation operation) {
    switch (operation) {
        case BinaryOperation::EQUALS:
            return " -eq ";
            break;
        case BinaryOperation::NOT_EQUALS:
            return " -ne ";
            break;
        case BinaryOperation::GREATER_THAN:
            return " -gt ";
            break;
        case BinaryOperation::GREATER_THAN_OR_EQUAL:
            return " -ge ";
            break;
        case BinaryOperation::LESS_THAN:
            return " -lt ";
            break;
        case BinaryOperation::LESS_THAN_OR_EQUAL:
            return " -le ";
            break;
        case BinaryOperation::ADD:
            return "+";
            break;
        case BinaryOperation::SUBTRACT:
            return "-";
            break;
        case BinaryOperation::MULTIPLY:
            return "*";
            break;
        case BinaryOperation::DIVIDE:
            return "/";
            break;
        case BinaryOperation::POWER:
            return "**";
            break;
        case BinaryOperation::ASSIGN:
            return "=";
            break;
        default:
            break;
    }
}

void BashPrinter::print_bash(std::ostream& stream) {
    for (auto& bsh : bash) {
        print_dispatcher(bsh.get(), stream);
        stream << "\n";
    }
}

// === SUBPRINT FUNCTIONS ===
void BashPrinter::print(BashBinaryExpression& node, std::ostream& stream) {
    std::stringstream output;
    if (node.type == VariableType::BOOL) {
        print_dispatcher(node.left.get(), output);
        output << get_operation(node.operation);
        print_dispatcher(node.right.get(), output);
        stream << wrap(output.str(), node.wrap_type);
        return;
    }

    if (node.type != VariableType::STRING) {
        output << "echo ";
        if (dynamic_cast<BashBinaryExpression*>(node.left.get()))
            node.left->wrap_type = WrapType::COMMAND_WRAP;

        print_dispatcher(node.left.get(), output);

        output << get_operation(node.operation);

        if (dynamic_cast<BashBinaryExpression*>(node.right.get()))
            node.right->wrap_type = WrapType::COMMAND_WRAP;

        print_dispatcher(node.right.get(), output);
        output << " | bc -l)";
        stream << wrap(output.str(), node.wrap_type);
    } else {
        if (dynamic_cast<BashLiteral*>(node.left.get()))
            node.left->wrap_type = WrapType::DOUBLE_QUOTE;

        print_dispatcher(node.left.get(), output);

        if (node.operation != BinaryOperation::ADD) {
            panic("Bash Generator cannot perform a non-addition operation between two strings");
        }

        if (dynamic_cast<BashLiteral*>(node.right.get()))
            node.right->wrap_type = WrapType::DOUBLE_QUOTE;

        print_dispatcher(node.right.get(), output);
        stream << wrap(output.str(), node.wrap_type);
    }
}
void BashPrinter::print(BashLiteral& node, std::ostream& stream) {
    stream << wrap(node.data, node.wrap_type);
}
void BashPrinter::print(BashFunctionCallExpression& node, std::ostream& stream) {}
void BashPrinter::print(BashVariableReference& node, std::ostream& stream) {
    stream << wrap(node.identifier, node.wrap_type);
}

void BashPrinter::print(BashAssignment& node, std::ostream& stream) {
    stream << node.identifier << "=";
    if (dynamic_cast<BashBinaryExpression*>(node.right.get())) {
        node.right->wrap_type = WrapType::COMMAND_WRAP;
    } else if (node.right->type == VariableType::STRING) {
        node.right->wrap_type = WrapType::DOUBLE_QUOTE;
    } else if (dynamic_cast<BashVariableReference*>(node.right.get())) {
        node.right->wrap_type = WrapType::DOUBLE_VARIABLE_WRAP;
    }
    print_dispatcher(node.right.get(), stream);
}
void BashPrinter::print(BashIfStatement& node, std::ostream& stream) {
    stream << "if [[ ";
    node.condition->wrap_type = WrapType::NONE;
    print_dispatcher(node.condition.get(), stream);
    stream << " ]]; then";
    for (auto& then : node.if_true) {
        print_dispatcher(then.get(), stream);
    }
}
void BashPrinter::print(BashEndIfStatement& node, std::ostream& stream) {
    stream << "fi";
}
void BashPrinter::print(BashFunctionCallStatement& node, std::ostream& stream) {
    std::size_t dot_position = node.identifier.find(' ');
    std::string first_section = (dot_position == std::string::npos)
                                    ? node.identifier
                                    : node.identifier.substr(0, dot_position);

    if (std::find(includes.begin(), includes.end(), first_section) !=
        includes.end()) { // we only care about if the first section is included, for example in
                          // "git add" we only care about "git"

        stream << node.identifier << " ";
        for (auto& arg : node.args) {
            arg->wrap_type = WrapType::NONE;
            print_dispatcher(arg.get(), stream);
            stream << " ";
        }
    }
}