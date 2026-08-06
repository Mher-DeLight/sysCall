#pragma once
#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

extern std::vector<std::string> includes;

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
    GreaterEqual,
    LessEqual,
    GreaterThan,
    LessThan,

    LParen,
    RParen,
    LBrace,
    RBrace,

    Comma,
    Period,

    Colon,
    Semicolon,
    Arrow,
    Ampersand,
    DoubleAmpersand,
    StraightLine,
    DoubleStraightLine,
    Exclamation,
    Hashtag,

    KeywordIf,
    KeywordElse,
    KeywordVartype,

    PrewordInclude,

    EndOfFile
};
enum class UnaryOperation { COMPLEMENT, INCREMENT, DECREMENT };
enum class BinaryOperation {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    POWER,
    ASSIGN,

    EQUALS,
    NOT_EQUALS,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL,
    LESS_THAN,
    LESS_THAN_OR_EQUAL,

    LOGICAL_AND,
    LOGICAL_OR,
    LOGICAL_XOR
};
enum class AssignOperation {
    ASSIGN,
    ADDASSIGN,
    SUBTRACTASSIGN,
    MULTIPLYASSIGN,
    DIVIDEASSIGN,
    POWERASSIGN
};

class SourceLocation {
public:
    int row = -1;
    int column = -1;

    SourceLocation(int r, int c) : row(r), column(c) {}
    SourceLocation() = default;
};

const std::unordered_map<std::string, TokenType> word_table{{{"", TokenType::None},
                                                             {"if", TokenType::KeywordIf},
                                                             {"else", TokenType::KeywordElse},
                                                             {"int", TokenType::KeywordVartype},
                                                             {"float", TokenType::KeywordVartype},
                                                             {"string", TokenType::KeywordVartype},
                                                             {"include", TokenType::PrewordInclude},

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
                                                             {"->", TokenType::Arrow},
                                                             {"!", TokenType::Exclamation},
                                                             {">", TokenType::GreaterThan},
                                                             {"<", TokenType::LessThan},
                                                             {">=", TokenType::GreaterEqual},
                                                             {"<=", TokenType::LessEqual},
                                                             {",", TokenType::Comma},
                                                             {"#", TokenType::Hashtag}}};

struct ExpressionInfo {
    VariableType type;
    bool isLValue = false;
    bool isConstant = false;

    ExpressionInfo(VariableType type_, bool isLValue_ = false, bool isConstant_ = false)
        : type(type_), isLValue(isLValue_), isConstant(isConstant_) {}
};

class Visitor;
class PrettyPrinter;

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(Visitor& visitor) = 0;
};
class ScopeBlock : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> children;

    ScopeBlock(std::vector<std::unique_ptr<ASTNode>> children_) : children(std::move(children_)) {}
    ScopeBlock() = default;
    void accept(Visitor& visitor) override;
};

class Expression : public ASTNode {
public:
    VariableType return_type = VariableType::VOID; // invalid until annotated by the ast
    SourceLocation location;

    virtual ~Expression() = default;
    virtual void accept(Visitor& visitor) = 0;

    Expression(const SourceLocation& lct) : location(lct) {}
};
class BinaryExpression : public Expression {
public:
    std::unique_ptr<Expression> left;
    BinaryOperation op;
    std::unique_ptr<Expression> right;

    void accept(Visitor& visitor) override;
    BinaryExpression(const SourceLocation& src, std::unique_ptr<Expression> l, BinaryOperation op_,
                     std::unique_ptr<Expression> r)
        : Expression(src), left(std::move(l)), op(op_), right(std::move(r)) {}
};
class FunctionCallExpr : public Expression {
public:
    std::string identifier;
    std::vector<std::unique_ptr<Expression>> args;

    void accept(Visitor& visitor) override;
    FunctionCallExpr(const SourceLocation& src, const std::string& identifier_,
                     std::vector<std::unique_ptr<Expression>> args_)
        : Expression(src), identifier(identifier_), args(std::move(args_)) {}
};
class VariableReference : public Expression {
public:
    std::string identifier;

    void accept(Visitor& visitor) override;
    VariableReference(const SourceLocation& src, const std::string& identifier_)
        : Expression(src), identifier(identifier_) {}
};
class UnaryExpression : public Expression {
public:
    std::unique_ptr<Expression> value;
    UnaryOperation op;

    void accept(Visitor& visitor) override;
    UnaryExpression(const SourceLocation& src, std::unique_ptr<Expression> value_,
                    UnaryOperation& op_)
        : Expression(src), value(std::move(value_)), op(op_) {}
};

class Statement : public ASTNode {
public:
    SourceLocation location;

    virtual ~Statement() = default;
    virtual void accept(Visitor& visitor) = 0;

    Statement(const SourceLocation& lct) : location(lct) {}
};
class ConditionalStatement : public Statement {
public:
    virtual ~ConditionalStatement() = default;
    virtual void accept(Visitor& visitor) = 0;

    ConditionalStatement(const SourceLocation& lct) : Statement(lct) {}
};
class IfStatement : public ConditionalStatement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<ScopeBlock> block;
    std::unique_ptr<ConditionalStatement> nextStatement;
    void accept(Visitor& visitor) override;

    IfStatement(const SourceLocation& lct, std::unique_ptr<Expression> condition_,
                std::unique_ptr<ScopeBlock> block_,
                std::unique_ptr<ConditionalStatement> nextStatement_)
        : ConditionalStatement(lct), condition(std::move(condition_)), block(std::move(block_)),
          nextStatement(std::move(nextStatement_)) {}
};

class Literal : public Expression {
public:
    virtual ~Literal() = default;

    Literal(const SourceLocation& lct) : Expression(lct) {}
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
class VariableDefinition : public Statement {
public:
    VariableType type;
    std::string identifier;
    std::unique_ptr<Expression> value;
    void accept(Visitor& visitor) override;

    VariableDefinition(SourceLocation& src, VariableType& type_, const std::string& identifier_,
                       std::unique_ptr<Expression> value_)
        : Statement(src), type(type_), identifier(identifier_), value(std::move(value_)) {}
};
class VariableReassignment : public Statement {
public:
    std::string identifier;
    std::unique_ptr<Expression> value;

    void accept(Visitor& visitor) override;

    VariableReassignment(SourceLocation& src, const std::string& identifier_,
                         std::unique_ptr<Expression> value_)
        : Statement(src), identifier(identifier_), value(std::move(value_)) {}
};

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(ScopeBlock& node) = 0;
    virtual void visit(StringLiteral& node) = 0;
    virtual void visit(FloatLiteral& node) = 0;
    virtual void visit(IntegerLiteral& node) = 0;
    virtual void visit(BooleanLiteral& node) = 0;
    virtual void visit(VoidLiteral& node) = 0;
    virtual void visit(VariableDefinition& node) = 0;
    virtual void visit(BinaryExpression& node) = 0;
    virtual void visit(IfStatement& node) = 0;
    virtual void visit(FunctionCallExpr& node) = 0;
    virtual void visit(VariableReference& node) = 0;
    virtual void visit(UnaryExpression& node) = 0;
    virtual void visit(VariableReassignment& node) = 0;
};
class PrettyPrinter : public Visitor {
private:
    int indent = 0;
    std::ostream& stream;
    void printIndent();

public:
    PrettyPrinter(std::ostream& stream_) : stream(stream_) {}

    void visit(ScopeBlock& node) override;
    void visit(StringLiteral& node) override;
    void visit(FloatLiteral& node) override;
    void visit(IntegerLiteral& node) override;
    void visit(BooleanLiteral& node) override;
    void visit(VoidLiteral& node) override;
    void visit(VariableDefinition& node) override;
    void visit(BinaryExpression& node) override;
    void visit(IfStatement& node) override;
    void visit(FunctionCallExpr& node) override;
    void visit(VariableReference& node) override;
    void visit(UnaryExpression& node) override;
    void visit(VariableReassignment& node) override;
};

inline void ScopeBlock::accept(Visitor& visitor) {
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
inline void BinaryExpression::accept(Visitor& visitor) {
    visitor.visit(*this);
}
inline void IfStatement::accept(Visitor& visitor) {
    visitor.visit(*this);
}
inline void FunctionCallExpr::accept(Visitor& visitor) {
    visitor.visit(*this);
}
inline void VariableReference::accept(Visitor& visitor) {
    visitor.visit(*this);
}
inline void UnaryExpression::accept(Visitor& visitor) {
    visitor.visit(*this);
}
inline void VariableReassignment::accept(Visitor& visitor) {
    visitor.visit(*this);
}