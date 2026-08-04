#pragma once
#include <string>
#include <vector>

class SourceLocation {
public:
    int row = -1;
    int column = -1;

    SourceLocation(int r, int c) : row(r), column(c) {}
};

enum class TokenType {
    Identifier,
    Number,
    String,

    Plus,
    Minus,
    Star,
    Slash,

    Equal,
    EqualEqual,
    NotEqual,

    LParen,
    RParen,

    Colon,
    Semicolon,
    Arrow,

    KeywordIf,
    KeywordElse,

    VartypeInt,
    VarypeFloat,
    VartypeString,

    EndOfFile
};

class Token {
public:
    TokenType type;
    std::string lexeme;
    SourceLocation location;

    Token(const TokenType type_, const std::string& lexeme_, const SourceLocation& source_location_)
        : type(type_), lexeme(lexeme_), location(source_location_) {}
};

class SysCAll_Tokenizer {
private:
    std::vector<Token> tokens;

public:
    void tokenize(const std::string& code);
    std::vector<Token>& get_tokens();
};
