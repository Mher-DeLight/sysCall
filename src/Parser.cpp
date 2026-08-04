#include "../include/Parser.h"
#include "../include/Common.h"
#include "../include/ErrorHandler.h"
#include "Tokenizer.h"

void Parser::load_tokens(std::vector<Token> tkns) {
    tokens = std::move(tkns);
}
void Parser::parse() {
    cursor = 0;

    while (!isEnd()) {
        if (check(TokenType::KeywordVartype))
            parseVariableDeclaration();
    }
}

// == PARSE FUNCTIONS ==
void Parser::parseVariableDeclaration() {
    std::string type =
        eat(TokenType::KeywordVartype, "expected variable type for variable declaration").lexeme;

    std::string identifier =
        eat(TokenType::Identifier, "expected identifier for variable declaration").lexeme;

    eat(TokenType::Equal);

    if (type == "int") {
        Token tkn = eat(TokenType::IntegerLiteral, "expected integer literal");
        entry_point.children.push_back(
            std::make_unique<IntegerLiteral>(tkn.location, std::stoi(tkn.lexeme)));
    } else if (type == "string") {
    } else if (type == "float") {
    } else if (type == "bool") {
    }
}

// == HELPERS ==
Token& Parser::peek(int offset) {
    if (cursor + offset > tokens.size()) {
        parserPanic("cannot peek into token number " + std::to_string(cursor + offset) +
                    "; such token does not exist.");
    }
    return tokens[cursor + offset];
}
Token& Parser::previous(int offset) {
    if (cursor - offset > tokens.size()) {
        parserPanic("cannot peek into previous token number " + std::to_string(cursor - offset) +
                    "; such token does not exist.");
    }
    return tokens[cursor - offset];
}
bool Parser::isEnd() {
    return cursor >= tokens.size();
}
bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    } else {
        return false;
    }
}
bool Parser::check(TokenType type) {
    if (peek().type == type) {
        return true;
    }
    return false;
}
void Parser::advance(int offset) {
    if (++cursor > tokens.size()) {
        parserPanic("cannot advance to position " + std::to_string(cursor) +
                    "; position is out of bounds.");
    }
}
Token& Parser::eat(TokenType type, const std::string& msg) {
    if (!check(type)) {
        parserPanic(msg, peek().location);
    } else {
        advance();
        return peek(0);
    }
    return peek(0);
}
void Parser::parserPanic(const std::string& msg, const SourceLocation& src) {
    panic("[PARSER PANIC] " + msg + " [AT " + std::to_string(src.row) + ":" +
          std::to_string(src.column) + "]");
}
