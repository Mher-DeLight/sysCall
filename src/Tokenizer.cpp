#include "../include/Tokenizer.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <ostream>
#include <string>

void Tokenizer::advance() {
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
TokenType Tokenizer::get_word_type(const std::string& str) {
    auto it = std::find_if(word_table.begin(), word_table.end(),
                           [&str](const auto& p) { return p.first == str; });
    if (it == word_table.end()) {
        return TokenType::Identifier;
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

void Tokenizer::tokenize(const std::string& c) {
    code = c;
    cursor = 0;
    int row = 1;
    int column = 1;

    while (cursor < code.size()) {
        if (current() == '\n') {
            row += 1;
            column = 1;
            advance();
            continue;
        }
        if (current() == ' ') {
            column++;
            advance();
            continue;
        }

        column += 1;

        if (std::isalpha(current()) || current() == '_') {
            std::string str;

            while (current() != ' ') {
                str += current();
                advance();
            }

            token(get_word_type(str), str, SourceLocation(row, column));
            advance();
            continue;
        }

        advance();
    }
}
