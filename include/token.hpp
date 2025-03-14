#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "all.hpp"

enum TOKEN_TYPE {
    IDENTIFIER,
    NUMBER,
    STRING,
    KEYWORD,
    OPERATOR,
    PUNCTUATOR,
    PARENTHESIS,
    COMMENT,
    PREPROCESSOR,
    WHITESPACE,
    NEWLINE,
    END_OF_FILE,
    UNKNOWN
};

enum tState {
    NewToken, AcceptToken,

    Identifier, Keyword,

    Number, String, CharLiteral,

    Add, Sub, Mul, Div, Mod,

    And, Or, Not, BitAnd, BitOr, BitXor, BitNot,

    Assign, Equal, NotEqual, Less, Greater, LessEqual, GreaterEqual,

    ShiftLeft, ShiftRight,

    Increment, Decrement,

    Ternary, Colon, Arrow,

    OpenParen, CloseParen, OpenBrace, CloseBrace, OpenBracket, CloseBracket,
    Comma, Semicolon, Dot,

    Preprocessor,

    If, Else, Switch, Case, Default, While, Do, For, Break, Continue, Return, Goto,

    Int, Char, Float, Double, Void, Bool, Short, Long, Signed, Unsigned,

    Auto, Const, Volatile, Extern, Static, Typedef,

    Enum, Struct, Union,

    True, False,

    Nullptr
};



class Token {
public:
    string lexeme;
    TOKEN_TYPE type;
    double magnitude;

    void set(string lexeme, TOKEN_TYPE type) {
        this->lexeme = lexeme;
        this->type = type;
    }

    void set(string lexeme, TOKEN_TYPE type, double magnitude = 0) {
        this->lexeme = lexeme;
        this->type = type;
        this->magnitude = magnitude;
    }
    
    void describe() {
        if (type == NUMBER) {
            cout << "Lexeme: " << to_string(magnitude);
        }
        cout << "Lexeme: " << lexeme;
        switch (type) {
            case IDENTIFIER:
                cout << ", Type: IDENTIFIER" << endl;
                break;
            case NUMBER:
                cout << ", Type: NUMBER" << endl;
                break;  
            case STRING:
                cout << ", Type: STRING" << endl;
                break;
            case KEYWORD:
                cout << ", Type: KEYWORD" << endl;
                break;
            case OPERATOR:
                cout << ", Type: OPERATOR" << endl;
                break;
            case PUNCTUATOR:
                cout << ", Type: PUNCTUATOR" << endl;
                break;
            case PARENTHESIS:
                cout << ", Type: PARENTHESIS" << endl;
                break;
            case COMMENT:
                cout << ", Type: COMMENT" << endl;
                break;
            case PREPROCESSOR:
                cout << ", Type: PREPROCESSOR" << endl;
                break;
            case WHITESPACE:
                cout << ", Type: WHITESPACE" << endl;
                break;
            case NEWLINE:
                cout << ", Type: NEWLINE" << endl;
                break;
            case END_OF_FILE:
                cout << ", Type: END_OF_FILE" << endl;
                break;
            default:
                cout << ", Type: UNKNOWN" << endl;
                break;  
        }
    }
};

#endif // TOKEN_HPP