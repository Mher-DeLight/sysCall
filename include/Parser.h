#pragma once
#include "Common.h"
#include "Tokenizer.h"
#include <memory>
#include <vector>

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
