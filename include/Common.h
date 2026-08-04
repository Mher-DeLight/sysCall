#pragma once

#include <array>
#include <string>
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
