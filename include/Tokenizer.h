#pragma once
#include <array>
#include <string>
#include <vector>

class SourceLocation {
public:
    int row = -1;
    int column = -1;

    SourceLocation(int r, int c) : row(r), column(c) {}
    SourceLocation() = default;
};

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

class Token {
public:
    TokenType type;
    std::string lexeme;
    SourceLocation location;

    Token(const TokenType type_, const std::string& lexeme_, const SourceLocation& source_location_)
        : type(type_), lexeme(lexeme_), location(source_location_) {}
};

class Tokenizer {
private:
    std::string code;
    std::vector<Token> tokens;

    int cursor = 0;
    int row = 1;
    int column = 1;

    TokenType get_word_type(const std::string& str, bool allow_identifiers = true);
    bool is_valid_position(const int pos) const;
    void assert_validity(const int pos, const std::string& error_message = "") const;
    bool is_identifier(const std::string& str);
    bool eof();
    void advance();
    char peek(int offset = 1) const;
    void token(const TokenType type, const std::string& lexeme_ = "",
               SourceLocation src = SourceLocation());
    char current() const;
    bool is_character(const char c);

public:
    void tokenize(const std::string& c);
    std::vector<Token>& get_tokens();
    void pretty_print(std::ostream& os);
};
