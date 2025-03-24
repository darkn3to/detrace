#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "all.hpp"
#include <stdbool.h>
#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <string>

enum TOKEN_TYPE {
    SYMBOL,
    IDENTIFIER,
    NUMBER,
    STRING,
    KEYWORD,
    OPERATOR,
    PUNCTUATOR,
    LPAREN,
    RPAREN,
    LSQBRACKET,
    RSQBRACKET,
    LBRACE,
    RBRACE,
    LABRACKET,
    RABRACKET,
    COLON,
    STRING_LITERAL,
    SEMICOLON,
    PREPROCESSOR,
    UNKNOWN,

    // Keywords
    AUTO,
    BREAK,
    CASE,
    CHAR,
    CONST,
    CONTINUE,
    DEFAULT,
    DO,
    DOUBLE,
    ELSE,
    ENUM,
    EXTERN,
    FLOAT,
    FOR,
    GOTO,
    IF,
    INT,
    LONG,
    RETURN,
    SHORT,
    SIGNED,
    SIZEOF,
    STATIC,
    STRUCT,
    SWITCH,
    TYPEDEF,
    UNION,
    UNSIGNED,
    VOID,
    VOLATILE,
    WHILE
};

enum tState {
    NewToken, AcceptToken,
    Identifier, Symbol,
    Number, String, CharLiteral,
    Add, Sub, Mul, Div, Mod,
    And, Or, Not, BitAnd, BitOr, BitXor, BitNot,
    Assign, Equal, NotEqual, Less, Greater, LessEqual, GreaterEqual,
    ShiftLeft, ShiftRight,
    Increment, Decrement,
    Ternary, Colon, Arrow,
    Comma, Dot,
    Preprocessor, Comment, 
    True, False,
    Nullptr, Unknown
};

unordered_map<string, TOKEN_TYPE> keywordMap = {
    {"auto", TOKEN_TYPE::AUTO},
    {"break", TOKEN_TYPE::BREAK},
    {"case", TOKEN_TYPE::CASE},
    {"char", TOKEN_TYPE::CHAR},
    {"const", TOKEN_TYPE::CONST},
    {"continue", TOKEN_TYPE::CONTINUE},
    {"default", TOKEN_TYPE::DEFAULT},
    {"do", TOKEN_TYPE::DO},
    {"double", TOKEN_TYPE::DOUBLE},
    {"else", TOKEN_TYPE::ELSE},
    {"enum", TOKEN_TYPE::ENUM},
    {"extern", TOKEN_TYPE::EXTERN},
    {"float", TOKEN_TYPE::FLOAT},
    {"for", TOKEN_TYPE::FOR},
    {"goto", TOKEN_TYPE::GOTO},
    {"if", TOKEN_TYPE::IF},
    {"int", TOKEN_TYPE::INT},
    {"long", TOKEN_TYPE::LONG},
    {"return", TOKEN_TYPE::RETURN},
    {"short", TOKEN_TYPE::SHORT},
    {"signed", TOKEN_TYPE::SIGNED},
    {"sizeof", TOKEN_TYPE::SIZEOF},
    {"static", TOKEN_TYPE::STATIC},
    {"struct", TOKEN_TYPE::STRUCT},
    {"switch", TOKEN_TYPE::SWITCH},
    {"typedef", TOKEN_TYPE::TYPEDEF},
    {"union", TOKEN_TYPE::UNION},
    {"unsigned", TOKEN_TYPE::UNSIGNED},
    {"void", TOKEN_TYPE::VOID},
    {"volatile", TOKEN_TYPE::VOLATILE},
    {"while", TOKEN_TYPE::WHILE}
};

string tokenTypeToString(TOKEN_TYPE type) {
    switch(type) {
        case SYMBOL:         return "SYMBOL";
        case IDENTIFIER:     return "IDENTIFIER";
        case NUMBER:         return "NUMBER";
        case STRING:         return "STRING";
        case KEYWORD:        return "KEYWORD";
        case OPERATOR:       return "OPERATOR";
        case PUNCTUATOR:     return "PUNCTUATOR";
        case LPAREN:         return "LPAREN";
        case RPAREN:         return "RPAREN";
        case LSQBRACKET:     return "LSQBRACKET";
        case RSQBRACKET:     return "RSQBRACKET";
        case LBRACE:         return "LBRACE";
        case RBRACE:         return "RBRACE";
        case LABRACKET:      return "LABRACKET";
        case RABRACKET:      return "RABRACKET";
        case COLON:          return "COLON";
        case STRING_LITERAL: return "STRING_LITERAL";
        case SEMICOLON:      return "SEMICOLON";
        case PREPROCESSOR:   return "PREPROCESSOR";
        case AUTO:           return "AUTO";
        case BREAK:          return "BREAK";
        case CASE:           return "CASE";
        case CHAR:           return "CHAR";
        case CONST:          return "CONST";
        case CONTINUE:       return "CONTINUE";
        case DEFAULT:        return "DEFAULT";
        case DO:             return "DO";
        case DOUBLE:         return "DOUBLE";
        case ELSE:           return "ELSE";
        case ENUM:           return "ENUM";
        case EXTERN:         return "EXTERN";
        case FLOAT:          return "FLOAT";
        case FOR:            return "FOR";
        case GOTO:           return "GOTO";
        case IF:             return "IF";
        case INT:            return "INT";
        case LONG:           return "LONG";
        case RETURN:         return "RETURN";
        case SHORT:          return "SHORT";
        case SIGNED:         return "SIGNED";
        case SIZEOF:         return "SIZEOF";
        case STATIC:         return "STATIC";
        case STRUCT:         return "STRUCT";
        case SWITCH:         return "SWITCH";
        case TYPEDEF:        return "TYPEDEF";
        case UNION:          return "UNION";
        case UNSIGNED:       return "UNSIGNED";
        case VOID:           return "VOID";
        case VOLATILE:       return "VOLATILE";
        case WHILE:          return "WHILE";
        default:             return "UNKNOWN";
    }
}

class Token {
public:
    string lexeme;
    TOKEN_TYPE type;
    double magnitude;

    void set(TOKEN_TYPE type) {
        this->type = type;
    }

    void set(string lexeme, TOKEN_TYPE type) {
        this->lexeme = lexeme;
        this->type = type;
    }

    void set(string lexeme, TOKEN_TYPE type, double magnitude) {
        this->lexeme = lexeme;
        this->type = type;
        this->magnitude = magnitude;
    }
    
    void describe(FILE *out) const {
        string typeStr = tokenTypeToString(type);
        fprintf(out, "%s ", typeStr.c_str());
    }
};

const inline bool is_Identifier_Start(char c) {
    return isalpha(c) || c=='_';
}

const inline bool is_Identifier(char c) {
    return isalnum(c) || c=='_';
}

#endif // TOKEN_HPP
