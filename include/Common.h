#pragma once
#include <array>
#include <memory>
#include <string>
#include <vector>
enum class VariableType { INT, FLOAT, STRING, CHAR, BOOL, VOID };

enum class TokenType {
    None,

    Identifier,
    StringLiteral,
    IntegerLiteral,
    FloatLiteral,
    CharLiteral,
    BoolLiteral,
    NullLiteral,

    Plus,
    Minus,
    Star,
    Slash,

    Equal,
    EqualEqual,
    NotEqual,

    LParen,
    RParen,
    LBrace,
    RBrace,

    Colon,
    Semicolon,
    Arrow,
    Ampersand,

    KeywordIf,
    KeywordElse,
    KeywordVartype,

    EndOfFile
};

class SourceLocation {
public:
    int row = -1;
    int column = -1;

    SourceLocation(int r, int c) : row(r), column(c) {}
    SourceLocation() = default;
};

const std::array<std::pair<std::string, TokenType>, 21> word_table{
    {{"", TokenType::None},
     {"if", TokenType::KeywordIf},
     {"else", TokenType::KeywordElse},
     {"int", TokenType::KeywordVartype},
     {"float", TokenType::KeywordVartype},
     {"string", TokenType::KeywordVartype},

     {"+", TokenType::Plus},
     {"-", TokenType::Minus},
     {"*", TokenType::Star},
     {"/", TokenType::Slash},

     {"=", TokenType::Equal},
     {"==", TokenType::EqualEqual},
     {"!=", TokenType::NotEqual},

     {"(", TokenType::LParen},
     {")", TokenType::RParen},
     {"{", TokenType::LBrace},
     {"}", TokenType::RBrace},
     {"&", TokenType::Ampersand},
     {":", TokenType::Colon},
     {";", TokenType::Semicolon},
     {"->", TokenType::Arrow}}};

class Visitor;
class PrettyPrinter;

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(Visitor& visitor) = 0;
};
class EntryPoint : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> children;

    EntryPoint(std::vector<std::unique_ptr<ASTNode>> children_) : children(std::move(children_)) {}
    EntryPoint() = default;
    void accept(Visitor& visitor) override;
};
class Literal : public ASTNode {
public:
    virtual ~Literal() = default;
    SourceLocation location;

    Literal(const SourceLocation& lct) : location(lct) {}
    Literal() = default;
    virtual void accept(Visitor& visitor) = 0;
};
class StringLiteral : public Literal {
public:
    std::string string;

    StringLiteral(const SourceLocation& lct, const std::string str = "")
        : Literal(lct), string(str) {}
    void accept(Visitor& visitor) override;
};
class FloatLiteral : public Literal {
public:
    float number;

    FloatLiteral(const SourceLocation& lct, const float number_) : Literal(lct), number(number_) {}
    void accept(Visitor& visitor) override;
};
class IntegerLiteral : public Literal {
public:
    int number;

    IntegerLiteral(const SourceLocation& lct, const float number_)
        : Literal(lct), number(number_) {}
    void accept(Visitor& visitor) override;
};
class BooleanLiteral : public Literal {
public:
    bool state;

    BooleanLiteral(const SourceLocation& lct, const bool state_) : Literal(lct), state(state_) {}
    void accept(Visitor& visitor) override;
};
class VoidLiteral : public Literal {
public:
    void accept(Visitor& visitor) override;
};

class VariableDefinition : public ASTNode {
public:
    VariableType type;
    std::string identifier;
    std::unique_ptr<Literal> value;
    void accept(Visitor& visitor) override;

    VariableDefinition(VariableType& type_, const std::string& identifier_,
                       std::unique_ptr<Literal> value_)
        : type(type_), identifier(identifier_), value(std::move(value_)) {}
};

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(EntryPoint& node) = 0;
    virtual void visit(StringLiteral& node) = 0;
    virtual void visit(FloatLiteral& node) = 0;
    virtual void visit(IntegerLiteral& node) = 0;
    virtual void visit(BooleanLiteral& node) = 0;
    virtual void visit(VoidLiteral& node) = 0;
    virtual void visit(VariableDefinition& node) = 0;
};

class PrettyPrinter : public Visitor {
private:
    int indent = 0;
    std::ostream& stream;
    void printIndent();

public:
    PrettyPrinter(std::ostream& stream_) : stream(stream_) {}

    void visit(EntryPoint& node) override;
    void visit(StringLiteral& node) override;
    void visit(FloatLiteral& node) override;
    void visit(IntegerLiteral& node) override;
    void visit(BooleanLiteral& node) override;
    void visit(VoidLiteral& node) override;
    void visit(VariableDefinition& node) override;
};

inline void EntryPoint::accept(Visitor& visitor) {
    visitor.visit(*this);
}
inline void StringLiteral::accept(Visitor& visitor) {
    visitor.visit(*this);
}
inline void FloatLiteral::accept(Visitor& visitor) {
    visitor.visit(*this);
}
inline void IntegerLiteral::accept(Visitor& visitor) {
    visitor.visit(*this);
}
inline void BooleanLiteral::accept(Visitor& visitor) {
    visitor.visit(*this);
}
inline void VoidLiteral::accept(Visitor& visitor) {
    visitor.visit(*this);
}
inline void VariableDefinition::accept(Visitor& visitor) {
    visitor.visit(*this);
}
