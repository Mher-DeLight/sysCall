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
    VartypeFloat,
    VartypeString,

    EndOfFile
};

const std::array<std::pair<std::string, TokenType>, 18> word_table{
    {{"", TokenType::None},
     {"if", TokenType::KeywordIf},
     {"else", TokenType::KeywordElse},
     {"int", TokenType::VartypeInt},
     {"float", TokenType::VartypeFloat},
     {"string", TokenType::VartypeString},

     {"+", TokenType::Plus},
     {"-", TokenType::Minus},
     {"*", TokenType::Star},
     {"/", TokenType::Slash},

     {"=", TokenType::Equal},
     {"==", TokenType::EqualEqual},
     {"!=", TokenType::NotEqual},

     {"(", TokenType::LParen},
     {")", TokenType::RParen},

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
