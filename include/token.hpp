#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "all.hpp"
#include <stdbool.h>
#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <string>

enum TOKEN_TYPE {
    SYMBOL = 1,
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
    CHAR_LITERAL,
    SEMICOLON,
    COMMA,
    DOT,
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
    WHILE,
    PLUS,      
    MINUS,    
    MULTIPLY, 
    DIVIDE,     
    MODULO,   
    ASSIGN,     
    PLUS_ASSIGN,
    MINUS_ASSIGN,
    NOT,        
    LESS,      
    GREATER,    
    BIT_AND,    
    BIT_OR,     
    EQUAL,      
    NOT_EQUAL,  
    LESS_EQUAL, 
    GREATER_EQUAL,
    AND,        
    OR,
    INCREMENT,
    DECREMENT,
    MUL_ASSIGN,
    DIV_ASSIGN,
    SHIFT_LEFT,
    SHIFT_RIGHT

};

enum tState {
    NewToken, AcceptToken,
    Identifier, Symbol,
    Number, String, CharLiteral,
    Plus, Minus, Multiply, Divide, Modulo,
    Assign, Not, Less, Greater,
    BitAnd, BitOr, Equal, NotEqual,
    LessEqual, GreaterEqual, And, Or,
    ShiftLeft, ShiftRight,
    Increment, Decrement,
    Ternary, Colon, Arrow,
    Comma, Dot,
    Preprocessor, Comment, 
    True, False,
    Nullptr, Unknown
};

unordered_map<string, TOKEN_TYPE> operatorMap = {
    {"+", TOKEN_TYPE::PLUS}, {"-", TOKEN_TYPE::MINUS}, {"*", TOKEN_TYPE::MULTIPLY}, {"/", TOKEN_TYPE::DIVIDE}, {"%", TOKEN_TYPE::MODULO},
    {"=", TOKEN_TYPE::ASSIGN}, {"!", TOKEN_TYPE::NOT}, {"<", TOKEN_TYPE::LESS}, {">", TOKEN_TYPE::GREATER}, {"&", TOKEN_TYPE::BIT_AND}, {"|", TOKEN_TYPE::BIT_OR},
    {"==", TOKEN_TYPE::EQUAL}, {"!=", TOKEN_TYPE::NOT_EQUAL}, {"<=", TOKEN_TYPE::LESS_EQUAL}, {">=", TOKEN_TYPE::GREATER_EQUAL}, {"&&", TOKEN_TYPE::AND}, {"||", TOKEN_TYPE::OR},
    {"++", TOKEN_TYPE::INCREMENT}, {"--", TOKEN_TYPE::DECREMENT}, {"+=", TOKEN_TYPE::PLUS_ASSIGN},
    {"-=", TOKEN_TYPE::MINUS_ASSIGN}, {"*=", TOKEN_TYPE::MUL_ASSIGN}, {"/=", TOKEN_TYPE::DIV_ASSIGN}, {">>", TOKEN_TYPE::SHIFT_LEFT}, {"<<", TOKEN_TYPE::SHIFT_RIGHT}
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
        case CHAR_LITERAL:   return "CHAR_LITERAL";
        case SEMICOLON:      return "SEMICOLON";
        case COMMA:          return "COMMA";
        case DOT:            return "DOT";
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
        case PLUS:          return "PLUS";
        case MINUS:         return "MINUS";
        case MULTIPLY:      return "MULTIPLY";
        case DIVIDE:        return "DIVIDE";
        case MODULO:        return "MODULO";
        case ASSIGN:        return "ASSIGN";
        case INCREMENT:      return "INCREMENT";
        case DECREMENT:      return "DECREMENT";
        case PLUS_ASSIGN:   return "PLUS_ASSIGN";
        case MINUS_ASSIGN:  return "MINUS_ASSIGN";
        case NOT:           return "NOT";
        case LESS:          return "LESS";
        case GREATER:       return "GREATER";
        case BIT_AND:       return "BIT_AND";
        case BIT_OR:        return "BIT_OR";
        case EQUAL:         return "EQUAL";
        case NOT_EQUAL:     return "NOT_EQUAL";
        case LESS_EQUAL:    return "LESS_EQUAL";
        case GREATER_EQUAL: return "GREATER_EQUAL";
        case AND:           return "AND";
        case OR:            return "OR";
        case MUL_ASSIGN:     return "MUL_ASSIGN";
        case DIV_ASSIGN:     return "DIV_ASSIGN";
        case SHIFT_LEFT:    return "SHIFT_LEFT";
        case SHIFT_RIGHT:   return "SHIFT_RIGHT";

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
    
    void describe(FILE *out, vector<unsigned short> &iToken) const {
        string typeStr = tokenTypeToString(type);
        //cout << "Lexeme: " << lexeme << " | Type: " << typeStr << endl;
        // Removed: if (flag) { buffer.clear(); flag = false; }
        //buffer += to_string(type) + " ";
        //cout << static_cast<int>(type) << " ";
        iToken.push_back(static_cast<unsigned short>(type));
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
