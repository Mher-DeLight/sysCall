#pragma once
#include "Tokenizer.h"
#include <memory>
#include <vector>

class ASTNode {
public:
    virtual ~ASTNode() = default;
};
class EntryPoint : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> children;

    EntryPoint(std::vector<std::unique_ptr<ASTNode>> children_) : children(std::move(children_)) {}
    EntryPoint() = default;
};

class Parser {
private:
    std::vector<Token> tokens;
    EntryPoint entry_point;

public:
    void load_tokens(const std::vector<Token>& tokens_);
    void parse();
    std::unique_ptr<EntryPoint> hand_over_AST();
};
