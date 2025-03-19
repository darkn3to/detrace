#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "all.hpp"
#include <stdbool.h>
#include <iostream>

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
    COMMENT,
    PREPROCESSOR,
    WHITESPACE,
    NEWLINE,
    END_OF_FILE,
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

const unordered_map<std::string, TOKEN_TYPE> keywordMap = {
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
    
    void describe() const {
        //std::cout << "Describing token: " << lexeme << std::endl;
        if (type == NUMBER) {
            cout << "Lexeme: " << to_string(magnitude) << endl;
        } 
        else {
            cout << "Lexeme: " << lexeme << endl;
        }
        switch (type) {
            case SYMBOL:
                cout << "Type: SYMBOL" << endl;
                cout << endl;
                break;
            case IDENTIFIER:
                cout << "Type: IDENTIFIER" << endl;
                cout << endl;
                break;
            case NUMBER:
                cout << "Type: NUMBER" << endl;
                cout << endl;
                break;  
            case STRING:
                cout << "Type: STRING" << endl;
                cout << endl;
                break;
            case STRING_LITERAL:
                cout << "Type: STRING_LITERAL" << endl;
                cout << endl;
                break;
            case KEYWORD:
                cout << "Type: KEYWORD" << endl;
                cout << endl;
                break;
            case OPERATOR:
                cout << "Type: OPERATOR" << endl;
                cout << endl;
                break;
            case PUNCTUATOR:
                cout << "Type: PUNCTUATOR" << endl;
                cout << endl;
                break;
            case LPAREN:
                cout << "Type: LPAREN" << endl;
                cout << endl;
                break;
            case RPAREN:
                cout << "Type: RPAREN" << endl;
                cout << endl;
                break;
            case LSQBRACKET:
                cout << "Type: LSQBRACKET" << endl;
                cout << endl;
                break;
            case RSQBRACKET:
                cout << "Type: RSQBRACKET" << endl;
                cout << endl;
                break;
            case LBRACE:
                cout << "Type: LBRACE" << endl;
                cout << endl;
                break;
            case RBRACE:
                cout << "Type: RBRACE" << endl;
                cout << endl;
                break;
            case COLON:
                cout << "Type: COLON" << endl;
                cout << endl;
                break;
            case SEMICOLON:
                cout << "Type: SEMICOLON" << endl;
                cout << endl;
                break;
            case COMMENT:
                cout << "Type: COMMENT" << endl;
                cout << endl;
                break;
            case PREPROCESSOR:
                cout << "Type: PREPROCESSOR" << endl;
                cout << endl;
                break;
            case WHITESPACE:
                cout << "Type: WHITESPACE" << endl;
                cout << endl;
                break;
            case NEWLINE:
                cout << "Type: NEWLINE" << endl;
                cout << endl;
                break;
            case END_OF_FILE:
                cout << "Type: END_OF_FILE" << endl;
                cout << endl;
                break;
            default:
                cout << "Type: UNKNOWN" << endl;
                cout << endl;
                break;  
        }
    }
};

bool is_Identifier_Start(char c) {
    return isalpha(c) || c=='_';
}

bool is_Identifier(char c) {
    return isalnum(c) || c=='_';
}

#endif // TOKEN_HPP