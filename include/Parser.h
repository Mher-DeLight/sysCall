#pragma once
#include "Common.h"
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
class Literal : public ASTNode {
public:
    virtual ~Literal() = default;
    SourceLocation location;

    Literal(const SourceLocation& lct) : location(lct) {}
    Literal() = default;
};
class StringLiteral : public Literal {
public:
    std::string string;

    StringLiteral(const SourceLocation& lct, const std::string str = "")
        : Literal(lct), string(str) {}
};
class FloatLiteral : public Literal {
public:
    float number;

    FloatLiteral(const SourceLocation& lct, const float number_) : Literal(lct), number(number_) {}
};
class IntegerLiteral : public Literal {
public:
    int number;

    IntegerLiteral(const SourceLocation& lct, const float number_)
        : Literal(lct), number(number_) {}
};
class BooleanLiteral : public Literal {
    bool state;

    BooleanLiteral(const SourceLocation& lct, const bool state_) : Literal(lct), state(state_) {}
};
class VoidLiteral : public Literal {};

class VariableDefinition : public ASTNode {
public:
    VariableType type;
    std::string identifier;
    std::unique_ptr<Literal> value;
};

class Parser {
private:
    std::vector<Token> tokens;
    EntryPoint entry_point;

    int cursor = 0;

    Token& peek(int offset = 1);
    Token& previous(int offset = 1);
    Token& eat(TokenType type, const std::string& msg = "");
    bool isEnd();
    bool match(TokenType type);
    bool check(TokenType type);
    void advance(int offset = 1);
    void parserPanic(const std::string& msg, const SourceLocation& location = SourceLocation());

    void parseVariableDeclaration();

public:
    void load_tokens(std::vector<Token> tokens_);
    void parse();
    std::unique_ptr<EntryPoint> hand_over_AST();
};
