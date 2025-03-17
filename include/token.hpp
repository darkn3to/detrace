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
    SEMICOLON,
    COMMENT,
    PREPROCESSOR,
    WHITESPACE,
    NEWLINE,
    END_OF_FILE,
    UNKNOWN,
};

enum tState {
    NewToken, AcceptToken,

    Identifier, Keyword, Symbol,

    Number, String, CharLiteral,

    Add, Sub, Mul, Div, Mod,

    And, Or, Not, BitAnd, BitOr, BitXor, BitNot,

    Assign, Equal, NotEqual, Less, Greater, LessEqual, GreaterEqual,

    ShiftLeft, ShiftRight,

    Increment, Decrement,

    Ternary, Colon, Arrow,

    Comma, Dot,

    Preprocessor, Comment, 

    If, Else, Switch, Case, Default, While, Do, For, Break, Continue, Return, Goto,

    Int, Char, Float, Double, Void, Bool, Short, Long, Signed, Unsigned,

    Auto, Const, Volatile, Extern, Static, Typedef,

    Enum, Struct, Union,

    True, False,

    Nullptr, Unknown
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
                cout << "Type: SYMBOL" << std::endl;
                cout << endl;
                break;
            case IDENTIFIER:
                cout << "Type: IDENTIFIER" << std::endl;
                cout << endl;
                break;
            case NUMBER:
                cout << "Type: NUMBER" << std::endl;
                cout << endl;
                break;  
            case STRING:
                cout << "Type: STRING" << std::endl;
                cout << endl;
                break;
            case KEYWORD:
                cout << "Type: KEYWORD" << std::endl;
                cout << endl;
                break;
            case OPERATOR:
                cout << "Type: OPERATOR" << std::endl;
                cout << endl;
                break;
            case PUNCTUATOR:
                cout << "Type: PUNCTUATOR" << std::endl;
                cout << endl;
                break;
            case LPAREN:
                cout << "Type: LPAREN" << std::endl;
                cout << endl;
                break;
            case RPAREN:
                cout << "Type: RPAREN" << std::endl;
                cout << endl;
                break;
            case LSQBRACKET:
                cout << "Type: LSQBRACKET" << std::endl;
                cout << endl;
                break;
            case RSQBRACKET:
                cout << "Type: RSQBRACKET" << std::endl;
                cout << endl;
                break;
            case LBRACE:
                cout << "Type: LBRACE" << std::endl;
                cout << endl;
                break;
            case RBRACE:
                cout << "Type: RBRACE" << std::endl;
                cout << endl;
                break;
            case COLON:
                cout << "Type: COLON" << std::endl;
                cout << endl;
                break;
            case SEMICOLON:
                cout << "Type: SEMICOLON" << std::endl;
                cout << endl;
                break;
            case COMMENT:
                cout << "Type: COMMENT" << std::endl;
                cout << endl;
                break;
            case PREPROCESSOR:
                cout << "Type: PREPROCESSOR" << std::endl;
                cout << endl;
                break;
            case WHITESPACE:
                cout << "Type: WHITESPACE" << std::endl;
                cout << endl;
                break;
            case NEWLINE:
                cout << "Type: NEWLINE" << std::endl;
                cout << endl;
                break;
            case END_OF_FILE:
                cout << "Type: END_OF_FILE" << std::endl;
                cout << endl;
                break;
            default:
                cout << "Type: UNKNOWN" << std::endl;
                cout << endl;
                break;  
        }
    }
};

bool is_Symbol_Start(char c) {
    return isalpha(c) || c=='_';
}

bool is_Symbol(char c) {
    return isalnum(c) || c=='_';
}

#endif // TOKEN_HPP