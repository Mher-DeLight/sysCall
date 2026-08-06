#include "../include/SemanticAnalyser.h"
#include "../include/ErrorHandler.h"

std::unique_ptr<ScopeBlock> SemanticAnalyser::hand_over_AST() {
    return std::move(ast);
}
void SemanticAnalyser::load_ast(uq<ScopeBlock> ast_) {
    ast = std::move(ast_);
}
void SemanticAnalyser::analyse() {
    ast->accept(*this);
}
void SemanticAnalyser::semaPanic(const std::string& msg, SourceLocation src) {
    if (src.row == -1) {
        panic("[SEMANTIC ANALYSIS PANIC] " + msg);
        return;
    }

    panic("[SEMANTIC ANALYSIS PANIC] " + msg + " [AT " + std::to_string(src.row) + ":" +
          std::to_string(src.column) + "]");
}
ExpressionInfo SemanticAnalyser::analyseExpression(Expression* expr) {
    if (auto lt = dynamic_cast<StringLiteral*>(expr)) {
        expr->return_type = VariableType::STRING;
        return ExpressionInfo(VariableType::STRING, false, true);
    } else if (auto lt = dynamic_cast<IntegerLiteral*>(expr)) {
        expr->return_type = VariableType::INT;
        return ExpressionInfo(VariableType::INT, false, true);
    } else if (auto lt = dynamic_cast<FloatLiteral*>(expr)) {
        expr->return_type = VariableType::FLOAT;
        return ExpressionInfo(VariableType::FLOAT, false, true);
    } else if (auto lt = dynamic_cast<BooleanLiteral*>(expr)) {
        expr->return_type = VariableType::BOOL;
        return ExpressionInfo(VariableType::BOOL, false, true);
    } else if (auto lt = dynamic_cast<VoidLiteral*>(expr)) {
        expr->return_type = VariableType::VOID;
        return ExpressionInfo(VariableType::VOID, false, true);
    } else if (auto var = dynamic_cast<VariableReference*>(expr)) {
        const Symbol* symbol = getSymbol(var->identifier);
        if (symbol == nullptr)
            semaPanic("undeclared variable \"" + var->identifier + "\"", var->location);
        expr->return_type = symbol->type;
        return ExpressionInfo(symbol->type, true, false);
    } else if (auto func = dynamic_cast<FunctionCallExpr*>(expr)) {
        const Symbol* symbol = getSymbol(func->identifier);
        if (symbol == nullptr)
            semaPanic("undeclared function \"" + func->identifier + "\"", func->location);
        expr->return_type = symbol->type;
        return ExpressionInfo(symbol->type, false, true);
    } else if (auto bexpr = dynamic_cast<BinaryExpression*>(expr)) {
        auto leftInfo = analyseExpression(bexpr->left.get());
        auto rightInfo = analyseExpression(bexpr->right.get());

        if (leftInfo.type == VariableType::VOID || rightInfo.type == VariableType::VOID) {
            semaPanic("variable of type void cannot be used in binary expression", bexpr->location);
        }

        VariableType returnType = leftInfo.type;
        if (leftInfo.type != rightInfo.type) {
            if (leftInfo.type == VariableType::INT && rightInfo.type == VariableType::FLOAT) {
                returnType = VariableType::FLOAT; // floats have higher "precedence"
                goto validOperation;
            }
            if (leftInfo.type == VariableType::FLOAT && rightInfo.type == VariableType::INT) {
                returnType = VariableType::FLOAT;
                goto validOperation;
            }

            semaPanic("invalid operation between types", bexpr->location);
        }
    validOperation:
        expr->return_type = returnType;
        return ExpressionInfo(returnType, false, true);
    } else {
        semaPanic("invalid expression", expr->location);
    }
    return ExpressionInfo(VariableType::VOID);
}

void SemanticAnalyser::enter_scope() {
    if (stack.size() > 0)
        stack.push_back(std::make_unique<Scope>(stack.back().get()));
    else
        stack.push_back(std::make_unique<Scope>(nullptr));
}
void SemanticAnalyser::exit_scope() {
    stack.pop_back();
}
void SemanticAnalyser::addSymbol(const Symbol& symbol) {
    stack.back()->symbols[symbol.identifier] = std::make_unique<Symbol>(symbol);
}
bool SemanticAnalyser::symbolExists(const std::string& identifier) const {
    Scope* scp = stack.back().get();
    while (scp != nullptr) {
        if (scp->symbols.contains(identifier))
            return true;

        scp = scp->parent;
    }
    return false;
}
VariableType SemanticAnalyser::getSymbolType(const std::string& identifier) const {
    Scope* scp = stack.back().get();
    while (scp != nullptr) {
        if (scp->symbols.contains(identifier))
            return scp->symbols[identifier]->type;

        scp = scp->parent;
    }
    return VariableType::VOID;
}
const Symbol* SemanticAnalyser::getSymbol(const std::string& identifier) const {
    Scope* scp = stack.back().get();
    while (scp != nullptr) {
        if (scp->symbols.contains(identifier))
            return scp->symbols[identifier].get();

        scp = scp->parent;
    }
    return nullptr;
}

// == VISIT ==
void SemanticAnalyser::visit(ScopeBlock& node) {
    enter_scope();
    for (auto& child : node.children) {
        child->accept(*this);
    }
    exit_scope();
}
void SemanticAnalyser::visit(StringLiteral& node) {
    // literally what could go wrong in a string literal (famous last words)
}
void SemanticAnalyser::visit(FloatLiteral& node) {}
void SemanticAnalyser::visit(IntegerLiteral& node) {}
void SemanticAnalyser::visit(BooleanLiteral& node) {}
void SemanticAnalyser::visit(VoidLiteral& node) {}
void SemanticAnalyser::visit(BinaryExpression& node) {
    analyseExpression(&node);
}
void SemanticAnalyser::visit(IfStatement& node) {
    node.condition->accept(*this);
    node.block->accept(*this);
    if (node.nextStatement != nullptr)
        node.nextStatement->accept(*this);
}
void SemanticAnalyser::visit(FunctionCallExpr& node) {
    analyseExpression(&node);
    for (auto& arg : node.args) {
        arg->accept(*this);
    }
}
void SemanticAnalyser::visit(VariableReference& node) {
    if (!symbolExists(node.identifier)) {
        semaPanic("cannot reference variable \"" + node.identifier + "\"; it does not exist.",
                  node.location);
    }
    if (getSymbol(node.identifier)->kind != SymbolKind::Variable) {
        semaPanic("\"" + node.identifier + "\" is used as a variable even though it is a function",
                  node.location);
    }
    analyseExpression(&node);
}
void SemanticAnalyser::visit(VariableReassignment& node) {
    if (!symbolExists(node.identifier)) {
        semaPanic("cannot reassign variable \"" + node.identifier + "\"; it does not exist.",
                  node.location);
    }
    if (getSymbol(node.identifier)->kind != SymbolKind::Variable) {
        semaPanic("\"" + node.identifier + "\" is used as a variable even though it is a function",
                  node.location);
    }
}
void SemanticAnalyser::visit(VariableDefinition& node) {
    addSymbol(
        Symbol(node.identifier, SymbolKind::Variable, node.type, std::vector<VariableType>()));
    analyseExpression(node.value.get());
}
void SemanticAnalyser::visit(UnaryExpression& node) {
    node.value->accept(*this);
}