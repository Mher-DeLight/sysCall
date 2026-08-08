#include "../include/BashPrinter.h"
#include "../include/Common.h"
#include "../include/ErrorHandler.h"
#include <algorithm>
#include <iostream>
#include <sstream>

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
    switch (wrap_type) {
        case WrapType::NONE:
            return string;
        case WrapType::DOUBLE_QUOTE:
            return "\"" + string + "\"";
        case WrapType::SINGLE_QUOTE:
            return "\'" + string + "\'";
        case WrapType::VARIABLE_WRAP:
            return "${" + string + "}";
        case WrapType::SINGLE_VARIABE_WRAP:
            return "\'${" + string + "}\'";
        case WrapType::DOUBLE_VARIABLE_WRAP:
            return "\"${" + string + "}\"";
        case WrapType::COMMAND_WRAP:
            return "$(" + string + ")";
        case WrapType::SINGLE_COMMAND_WRAP:
            return "\'$(" + string + ")\'";
        case WrapType::DOUBLE_COMMAND_WRAP:
            return "\"$(" + string + ")\"";
    }
    return string;
}

std::string BashPrinter::get_operation(BinaryOperation operation) {
    switch (operation) {
        case BinaryOperation::EQUALS:
            return " -eq ";
        case BinaryOperation::NOT_EQUALS:
            return " -ne ";
        case BinaryOperation::GREATER_THAN:
            return " -gt ";
        case BinaryOperation::GREATER_THAN_OR_EQUAL:
            return " -ge ";
        case BinaryOperation::LESS_THAN:
            return " -lt ";
        case BinaryOperation::LESS_THAN_OR_EQUAL:
            return " -le ";
        case BinaryOperation::ADD:
            return " + ";
        case BinaryOperation::SUBTRACT:
            return " - ";
        case BinaryOperation::MULTIPLY:
            return " * ";
        case BinaryOperation::DIVIDE:
            return " / ";
        case BinaryOperation::POWER:
            return " ** ";
        case BinaryOperation::ASSIGN:
            return "=";
        case BinaryOperation::LOGICAL_AND:
            return " && ";
        case BinaryOperation::LOGICAL_OR:
            return " || ";
        case BinaryOperation::LOGICAL_XOR:
            return " ^ ";
        default:
            return " ";
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
    if (node.type == VariableType::BOOL) {
        std::ostringstream output;

        if (auto* lft = dynamic_cast<BashBinaryExpression*>(node.left.get())) {
            if (lft->type == VariableType::INT) {
                lft->wrap_type = WrapType::COMMAND_WRAP;
            }
        }
        if (auto* lft = dynamic_cast<BashLiteral*>(node.left.get())) {
            if (lft->type == VariableType::STRING) {
                lft->wrap_type = WrapType::DOUBLE_QUOTE;
            }
        }
        print_dispatcher(node.left.get(), output);

        if (node.left->type == VariableType::STRING && node.right->type == VariableType::STRING) {
            if (node.operation == BinaryOperation::EQUALS) {
                output << " == ";
            } else if (node.operation == BinaryOperation::NOT_EQUALS) {
                output << " != ";
            } else {
                panic("Bash Printer cannot print non-equality check operator for two strings");
            }
        } else {
            output << get_operation(node.operation);
        }
        if (auto* rgt = dynamic_cast<BashBinaryExpression*>(node.right.get())) {
            if (rgt->type == VariableType::INT) {
                rgt->wrap_type = WrapType::COMMAND_WRAP;
            }
        }
        if (auto* rgt = dynamic_cast<BashLiteral*>(node.right.get())) {
            if (rgt->type == VariableType::STRING) {
                rgt->wrap_type = WrapType::DOUBLE_QUOTE;
            }
        }
        print_dispatcher(node.right.get(), output);
        stream << wrap(output.str(), node.wrap_type);
        return;
    }

    if (node.type == VariableType::STRING) {
        if (node.operation != BinaryOperation::ADD) {
            panic("Bash Generator cannot perform a non-addition operation between two strings");
        }

        std::ostringstream output;
        if (auto* literal = dynamic_cast<BashLiteral*>(node.left.get())) {
            literal->wrap_type = WrapType::DOUBLE_QUOTE;
        } else if (auto* variable = dynamic_cast<BashVariableReference*>(node.left.get())) {
            variable->wrap_type = WrapType::VARIABLE_WRAP;
        } else if (auto* binar_expr = dynamic_cast<BashBinaryExpression*>(node.left.get())) {
            if (binar_expr->left->type == VariableType::STRING)
                binar_expr->wrap_type = WrapType::DOUBLE_QUOTE;
            else
                binar_expr->wrap_type = WrapType::COMMAND_WRAP;
        }
        print_dispatcher(node.left.get(), output);

        if (auto* literal = dynamic_cast<BashLiteral*>(node.right.get())) {
            literal->wrap_type = WrapType::DOUBLE_QUOTE;
        } else if (auto* variable = dynamic_cast<BashVariableReference*>(node.right.get())) {
            variable->wrap_type = WrapType::VARIABLE_WRAP;
        } else if (auto* binar_expr = dynamic_cast<BashBinaryExpression*>(node.right.get())) {
            binar_expr->wrap_type = WrapType::COMMAND_WRAP;
            if (binar_expr->left->type == VariableType::STRING)
                binar_expr->wrap_type = WrapType::DOUBLE_QUOTE;
            else
                binar_expr->wrap_type = WrapType::COMMAND_WRAP;
        }
        print_dispatcher(node.right.get(), output);

        stream << wrap(output.str(), node.wrap_type);
        return;
    }

    std::ostringstream output;
    output << "bc -l <<< \"";
    if (auto* binexpr = dynamic_cast<BashBinaryExpression*>(node.left.get())) {
        if (binexpr->type != VariableType::STRING) {
            node.left->wrap_type = WrapType::COMMAND_WRAP;
        }
    }
    print_dispatcher(node.left.get(), output);
    output << get_operation(node.operation);
    if (auto* binexpr = dynamic_cast<BashBinaryExpression*>(node.right.get())) {
        if (binexpr->type != VariableType::STRING) {
            node.left->wrap_type = WrapType::COMMAND_WRAP;
        }
    }
    print_dispatcher(node.right.get(), output);
    output << "\"";

    stream << wrap(output.str(), node.wrap_type);
}

void BashPrinter::print(BashLiteral& node, std::ostream& stream) {
    stream << wrap(node.data, node.wrap_type);
}

void BashPrinter::print(BashFunctionCallExpression& node, std::ostream& stream) {
    stream << node.identifier;
    for (auto& arg : node.args) {
        stream << " ";
        print_dispatcher(arg.get(), stream);
    }
}

void BashPrinter::print(BashVariableReference& node, std::ostream& stream) {
    stream << wrap(node.identifier, node.wrap_type);
}

void BashPrinter::print(BashAssignment& node, std::ostream& stream) {
    stream << node.identifier << "=";
    if (node.right->type == VariableType::INT || node.right->type == VariableType::FLOAT) {
        node.right->wrap_type = WrapType::NONE;
        print_dispatcher(node.right.get(), stream);
    } else if (node.right->type == VariableType::STRING || node.right->type == VariableType::BOOL) {
        node.right->wrap_type = WrapType::DOUBLE_QUOTE;
        print_dispatcher(node.right.get(), stream);
    }
}

void BashPrinter::print(BashIfStatement& node, std::ostream& stream) {
    stream << "if [[ ";
    if (node.condition->type != VariableType::BOOL)
        node.condition->wrap_type = WrapType::COMMAND_WRAP; // cuz it would use bc -l
    else
        node.condition->wrap_type = WrapType::NONE;
    print_dispatcher(node.condition.get(), stream);

    stream << " ]]; then";
    for (auto& then : node.if_true) {
        stream << "\n";
        print_dispatcher(then.get(), stream);
    }
}

void BashPrinter::print(BashEndIfStatement& node, std::ostream& stream) {
    (void)node;
    stream << "fi";
}

void BashPrinter::print(BashFunctionCallStatement& node, std::ostream& stream) {
    std::size_t dot_position = node.identifier.find(' ');
    std::string first_section = (dot_position == std::string::npos)
                                    ? node.identifier
                                    : node.identifier.substr(0, dot_position);

    if (std::find(includes.begin(), includes.end(), first_section) != includes.end()) {
        stream << node.identifier << " ";
        for (auto& arg : node.args) {
            arg->wrap_type = WrapType::NONE;
            if (auto* bexpr = dynamic_cast<BashBinaryExpression*>(arg.get())) {
                if (bexpr->type != VariableType::STRING) {
                    arg->wrap_type = WrapType::COMMAND_WRAP;
                }
            }
            print_dispatcher(arg.get(), stream);
            stream << " ";
        }
    }
}
