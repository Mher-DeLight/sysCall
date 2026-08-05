#include "../include/Parser.h"
#include "../include/Common.h"
#include "../include/ErrorHandler.h"
#include "Tokenizer.h"
#include <iostream>

void Parser::load_tokens(std::vector<Token> tkns) {
    tokens = std::move(tkns);
}
void Parser::parse() {
    cursor = -1;

    while (!isEnd()) {
        std::cout << (int)peek().type << std::endl;
        if (check(TokenType::KeywordVartype)) {
            parseVariableDeclaration();
        }

        advance();
    }

    PrettyPrinter p(std::cout);
    p.visit(entry_point);
}

// == PARSE FUNCTIONS ==
void Parser::parseVariableDeclaration() {
    Token tkn = eat(TokenType::KeywordVartype, "expected variable type for variable declaration");

    std::string type_string = tkn.lexeme;
    VariableType type = VariableType::VOID;

    if (type_string == "int")
        type = VariableType::INT;
    else if (type_string == "string")
        type = VariableType::STRING;
    else if (type_string == "float")
        type = VariableType::FLOAT;
    else if (type_string == "bool")
        type = VariableType::BOOL;

    std::string identifier =
        eat(TokenType::Identifier, "expected identifier for variable declaration").lexeme;

    eat(TokenType::Equal);

    std::unique_ptr<Literal> value;

    switch (type) {
        case VariableType::INT: {
            Token tkn = eat(TokenType::IntegerLiteral, "expected integer literal");
            value =
                std::move(std::make_unique<IntegerLiteral>(tkn.location, std::stoi(tkn.lexeme)));
            break;
        }
        case VariableType::FLOAT: {
            Token tkn = eat(TokenType::FloatLiteral, "expected float literal");
            value = std::move(std::make_unique<FloatLiteral>(tkn.location, std::stof(tkn.lexeme)));
            break;
        }
        case VariableType::STRING: {
            Token tkn = eat(TokenType::StringLiteral, "expected string literal");
            value = std::move(std::make_unique<StringLiteral>(tkn.location, tkn.lexeme));
            break;
        }
        default: {
            parserPanic("invalid variable type \"" + type_string + "\"", tkn.location);
        }
    }

    entry_point.children.push_back(
        std::make_unique<VariableDefinition>(type, identifier, std::move(value)));
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
    return cursor >= std::ssize(tokens);
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
