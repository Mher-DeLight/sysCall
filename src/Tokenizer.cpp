#include "../include/Tokenizer.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>

void Tokenizer::advance() {
    if (peek() == '\n') {
        column = 1;
        row++;
    } else {
        column++;
    }
    cursor++;
}
char Tokenizer::peek(int offset) const {
    return code[cursor + offset];
}
char Tokenizer::current() const {
    return code[cursor];
}
void Tokenizer::token(const TokenType tkn, const std::string& lexeme_, SourceLocation lct) {
    tokens.push_back(Token(tkn, lexeme_, lct));
}
bool Tokenizer::eof() {
    return cursor >= code.size();
}
bool Tokenizer::is_identifier(const std::string& str) {
    auto it = std::find_if(word_table.begin(), word_table.end(),
                           [&str](const auto& p) { return p.first == str; });
    return it == word_table.end();
}
TokenType Tokenizer::get_word_type(const std::string& str, bool allow_identifiers) {
    auto it = std::find_if(word_table.begin(), word_table.end(),
                           [&str](const auto& p) { return p.first == str; });
    if (it == word_table.end()) {
        if (allow_identifiers)
            return TokenType::Identifier;
        else
            return TokenType::None;
    } else {
        return it->second;
    }
    return TokenType::None;
}

void Tokenizer::pretty_print(std::ostream& os) {
    os << "=== TOKENS ===\n";
    for (auto& tkn : tokens) {
        os << "Token(" << (int)tkn.type << ", ";
        os << "\"" << tkn.lexeme << "\") ";
        os << tkn.location.row << ":" << tkn.location.column << "\n";
    }
}
bool Tokenizer::is_character(const char c) {
    return (std::string_view(".,:;-><+-/=!").find(c) != std::string_view::npos);
}

void Tokenizer::tokenize(const std::string& c) {
    code = c;
    cursor = 0;
    row = 1;
    column = 1;

    bool first_iteration = true;
    while (cursor < code.size()) {
        if (!first_iteration)
            advance();
        else
            first_iteration = false;

        if (current() == '\n') {
            continue;
        }
        if (current() == ' ') {
            continue;
        }

        SourceLocation location(row, column);
        if (std::isalpha(current()) || current() == '_') {
            std::string str;

            while (std::isalnum(current()) || current() == '_') {
                str += current();
                advance();
            }

            token(get_word_type(str), str, location);
            continue;
        } else if (is_character(current())) {
            std::string str;

            while (is_character(current())) {
                str += current();
                advance();
            }

            token(get_word_type(str, false), str, location);
            continue;
        } else if (std::isdigit(current())) {
            std::string str;

            bool found_dot = false;
            while (std::isdigit(current()) || (!found_dot && current() == '.')) {
                str += current();
                if (current() == '.')
                    found_dot = true;
                advance();
            }

            token(TokenType::Number, str, location);
            continue;
        }
    }
}
