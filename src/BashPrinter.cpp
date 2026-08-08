#include "../include/BashPrinter.h"
#include "../include/Common.h"
#include "../include/ErrorHandler.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

void BashPrinter::load_bash(std::vector<std::unique_ptr<BashStatement>> bsh) {
    bash = std::move(bsh);
}
void BashPrinter::print_dispatcher(Bash* node, std::ostream& stream) {
    if (!node) {
        panic("invalid pointer to node");
    }

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
    } else if (auto nd = dynamic_cast<BashElseIfStatement*>(node)) {
        return print(*nd, stream);
    } else if (auto nd = dynamic_cast<BashElseStatement*>(node)) {
        return print(*nd, stream);
    } else if (auto nd = dynamic_cast<BashForLoop*>(node)) {
        return print(*nd, stream);
    } else if (auto nd = dynamic_cast<BashDone*>(node)) {
        return print(*nd, stream);
    } else if (auto nd = dynamic_cast<BashUnaryExpression*>(node)) {
        return print(*nd, stream);
    } else {
        panic("invalid node for bash generator");
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
            return use_ugly_flags ? " -eq " : " == ";
        case BinaryOperation::NOT_EQUALS:
            return use_ugly_flags ? " -ne " : " != ";
        case BinaryOperation::GREATER_THAN:
            return use_ugly_flags ? " -gt " : " > ";
        case BinaryOperation::GREATER_THAN_OR_EQUAL:
            return use_ugly_flags ? " -ge " : " >= ";
        case BinaryOperation::LESS_THAN:
            return use_ugly_flags ? " -lt " : " < ";
        case BinaryOperation::LESS_THAN_OR_EQUAL:
            return use_ugly_flags ? " -le " : " <= ";
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
    stream << "#!/usr/bin/env bash\n";
    stream << "# This is an auto-generated bash script.\n";
    stream << "# Some parts may be optimized while others may be verbose.\n";
    stream << "# This script was not optimized for readability. If possible, read the .scl script "
              "that generated it.\n";
    stream << "# == generated via sysCall == https://www.github.com/Mher-DeLight/sysCall ==\n\n";
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

        if ((node.left->type == VariableType::STRING && node.right->type == VariableType::STRING) ||
            node.left->type == VariableType::BOOL && node.right->type == VariableType::BOOL) {
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
    if (use_bc_arithmetic) {
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
    } else {
        output << "$((";

        print_dispatcher(node.left.get(), output);
        output << get_operation(node.operation);
        print_dispatcher(node.right.get(), output);

        output << "))";
        stream << wrap(output.str(), node.wrap_type);
    }
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
}
void BashPrinter::print(BashEndIfStatement& node, std::ostream& stream) {
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
            if (auto* bexpr = dynamic_cast<BashVariableReference*>(arg.get())) {
                arg->wrap_type = WrapType::VARIABLE_WRAP;
            }
            print_dispatcher(arg.get(), stream);
            stream << " ";
        }
    }
}
void BashPrinter::print(BashElseIfStatement& node, std::ostream& stream) {
    stream << "elif [[ ";
    if (node.condition->type != VariableType::BOOL)
        node.condition->wrap_type = WrapType::COMMAND_WRAP; // cuz it would use bc -l
    else
        node.condition->wrap_type = WrapType::NONE;
    print_dispatcher(node.condition.get(), stream);

    stream << " ]]; then";
}
void BashPrinter::print(BashElseStatement& node, std::ostream& stream) {
    stream << "else";
}
void BashPrinter::print(BashForLoop& node, std::ostream& stream) {
    stream << "for ((";
    print_dispatcher(node.assignment.get(), stream);
    stream << "; ";

    node.condition->wrap_type = WrapType::NONE;
    use_ugly_flags = false;
    use_bc_arithmetic = false;
    print_dispatcher(node.condition.get(), stream);
    use_ugly_flags = true;

    stream << "; ";
    print_dispatcher(node.then_do.get(), stream);
    use_bc_arithmetic = false;
    stream << ")); do";
}
void BashPrinter::print(BashDone& node, std::ostream& stream) {
    stream << "done";
}
void BashPrinter::print(BashUnaryExpression& node, std::ostream& stream) {
    std::string unop = "";
    bool space = false;
    switch (node.operation) {
        case UnaryOperation::NEGATE:
            unop = "-";
            break;
        case UnaryOperation::COMPLEMENT:
            unop = "!";
            space = true;
            break;
        case UnaryOperation::INCREMENT:
            unop = "++";
            break;
        case UnaryOperation::DECREMENT:
            unop = "--";
            break;
        default:
            panic("invalid unary operation");
            break;
    }

    std::stringstream output;
    output << "$((";
    output << unop;
    if (space)
        output << " ";
    if (dynamic_cast<BashVariableReference*>(node.value.get()))
        node.value->wrap_type = WrapType::NONE; // we want to modify the variable, not its value
    print_dispatcher(node.value.get(), output);

    output << "))";
    stream << wrap(output.str(), node.wrap_type);
}