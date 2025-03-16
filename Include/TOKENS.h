#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
using namespace std ; 

unordered_map<string,vector<pair<string,string>>> TOKENS ; 

vector<pair<string,string>> keywords = {
    {"alignas", "ALIGNAS"}, {"alignof", "ALIGNOF"}, {"and", "LOGICAL_AND"}, {"and_eq", "BITWISE_AND_ASSIGN"},
    {"asm", "ASM"}, {"atomic_cancel", "ATOMIC_CANCEL"}, {"atomic_commit", "ATOMIC_COMMIT"},
    {"atomic_noexcept", "ATOMIC_NOEXCEPT"}, {"auto", "AUTO"}, {"bitand", "BITWISE_AND"},
    {"bitor", "BITWISE_OR"}, {"bool", "BOOL"}, {"break", "BREAK"}, {"case", "CASE"},
    {"catch", "CATCH"}, {"char", "CHAR"}, {"char8_t", "CHAR8_T"}, {"char16_t", "CHAR16_T"},
    {"char32_t", "CHAR32_T"}, {"class", "CLASS"}, {"compl", "BITWISE_NOT"}, {"concept", "CONCEPT"},
    {"const", "CONST"}, {"consteval", "CONSTEVAL"}, {"constexpr", "CONSTEXPR"},
    {"constinit", "CONSTINIT"}, {"const_cast", "CONST_CAST"}, {"continue", "CONTINUE"},
    {"contract_assert", "CONTRACT_ASSERT"}, {"co_await", "CO_AWAIT"}, {"co_return", "CO_RETURN"},
    {"co_yield", "CO_YIELD"}, {"decltype", "DECLTYPE"}, {"default", "DEFAULT"},
    {"delete", "DELETE"}, {"do", "DO"}, {"double", "DOUBLE"}, {"dynamic_cast", "DYNAMIC_CAST"},
    {"else", "ELSE"}, {"enum", "ENUM"}, {"explicit", "EXPLICIT"}, {"export", "EXPORT"},
    {"extern", "EXTERN"}, {"false", "FALSE"}, {"float", "FLOAT"}, {"for", "FOR"},
    {"friend", "FRIEND"}, {"goto", "GOTO"}, {"if", "IF"}, {"inline", "INLINE"},
    {"int", "INT"}, {"long", "LONG"}, {"mutable", "MUTABLE"}, {"namespace", "NAMESPACE"},
    {"new", "NEW"}, {"noexcept", "NOEXCEPT"}, {"not", "LOGICAL_NOT"}, {"not_eq", "NOT_EQUAL"},
    {"nullptr", "NULLPTR"}, {"operator", "OPERATOR"}, {"or", "LOGICAL_OR"}, {"or_eq", "BITWISE_OR_ASSIGN"},
    {"private", "PRIVATE"}, {"protected", "PROTECTED"}, {"public", "PUBLIC"},
    {"reflexpr", "REFLEXPR"}, {"register", "REGISTER"}, {"reinterpret_cast", "REINTERPRET_CAST"},
    {"requires", "REQUIRES"}, {"return", "RETURN"}, {"short", "SHORT"}, {"signed", "SIGNED"},
    {"sizeof", "SIZEOF"}, {"static", "STATIC"}, {"static_assert", "STATIC_ASSERT"},
    {"static_cast", "STATIC_CAST"}, {"struct", "STRUCT"}, {"switch", "SWITCH"},
    {"synchronized", "SYNCHRONIZED"}, {"template", "TEMPLATE"}, {"this", "THIS"},
    {"thread_local", "THREAD_LOCAL"}, {"throw", "THROW"}, {"true", "TRUE"}, {"try", "TRY"},
    {"typedef", "TYPEDEF"}, {"typeid", "TYPEID"}, {"typename", "TYPENAME"}, {"union", "UNION"},
    {"unsigned", "UNSIGNED"}, {"using", "USING"}, {"virtual", "VIRTUAL"}, {"void", "VOID"},
    {"volatile", "VOLATILE"}, {"wchar_t", "WCHAR_T"}, {"while", "WHILE"}, {"xor", "BITWISE_XOR"},
    {"xor_eq", "BITWISE_XOR_ASSIGN"},
    
    {"final", "FINAL"},
    {"override", "OVERRIDE"},
    {"import", "IMPORT"},
    {"module", "MODULE"},
    {"pre", "PRE"},
    {"post", "POST"}
};


vector<pair<string, string>> operators = {
	// Arithmetic operators ( +, -, *, /, % )
    {"+", "ADD"}, {"-", "SUBTRACT"}, {"*", "MULTIPLY"}, {"/", "DIVIDE"}, {"%", "MODULUS"},
    
    // Increment and decrement (++, --)
    {"++", "INCREMENT"}, {"--", "DECREMENT"}, 

	// Relational and comparison operators ( ==, !=, >, <, >=, <= )
    {"==", "EQUAL"}, {"!=", "NOT_EQUAL"}, {">", "GREATER_THAN"}, {"<", "LESS_THAN"},
    {">=", "GREATER_THAN_OR_EQUAL"}, {"<=", "LESS_THAN_OR_EQUAL"}, 

	// Logical operators ( !, &&, || )
    {"&&", "LOGICAL_AND"}, {"||", "LOGICAL_OR"}, {"!", "LOGICAL_NOT"}, 

	// Bitwise operators ( &, |, ^, ~, <<, >> )
    {"&", "BITWISE_AND"}, {"|", "BITWISE_OR"}, {"^", "BITWISE_XOR"}, {"~", "BITWISE_NOT"},
    {"<<", "BITWISE_LEFT_SHIFT"}, {">>", "BITWISE_RIGHT_SHIFT"},

	// Compound assignment (+=, -=, *=, /=, %=, >>=, <<=, &=, ^=, |=)
    {"+=", "ADD_ASSIGN"}, {"-=", "SUBTRACT_ASSIGN"}, {"*=", "MULTIPLY_ASSIGN"},
    {"/=", "DIVIDE_ASSIGN"}, {"%=", "MODULUS_ASSIGN"}, {"&=", "BITWISE_AND_ASSIGN"},
    {"|=", "BITWISE_OR_ASSIGN"}, {"^=", "BITWISE_XOR_ASSIGN"}, {"<<=", "LEFT_SHIFT_ASSIGN"},
    {">>=", "RIGHT_SHIFT_ASSIGN"},


    {"=", "ASSIGNMENT"}, {"?", "TERNARY_CONDITIONAL"}, {":", "TERNARY_SEPARATOR"},

    {"sizeof", "SIZEOF"}, {"typeid", "TYPEID"}, {"alignof", "ALIGNOF"},

    {"decltype", "DECLTYPE"}, {"constexpr", "CONSTEXPR"},

    {"co_await", "CO_AWAIT"}, {"co_yield", "CO_YIELD"}, {"co_return", "CO_RETURN"}
    
    //  {"#", "STRINGIZING_OPERATOR"}, {"##", "TOKEN_PASTING_OPERATOR"}
};

vector<pair<string, string>> preprocessors = {
    {"#include", "INCLUDE_HEADER"}, 
    {"__has_include", "HAS_INCLUDE"}, // C++17

    {"#define", "DEFINE_MACRO"}, 
    {"#undef", "UNDEFINE_MACRO"}, 

    {"#if", "IF_CONDITIONAL"}, 
    {"#ifdef", "IF_DEFINED"}, 
    {"#ifndef", "IF_NOT_DEFINED"}, 
    {"#else", "ELSE_CONDITIONAL"}, 
    {"#elif", "ELIF_CONDITIONAL"}, 
    {"#elifdef", "ELIF_DEFINED"}, // C++23
    {"#elifndef", "ELIF_NOT_DEFINED"}, // C++23
    {"#endif", "END_IF_CONDITIONAL"}, 

    {"#error", "ERROR_MESSAGE"}, 
    {"#warning", "WARNING_MESSAGE"}, // C++23

    {"#pragma", "PRAGMA_DIRECTIVE"}, 
    {"_Pragma", "PRAGMA_OPERATOR"}, // C++11

    {"#line", "SET_LINE_NUMBER"}, 
    {"#embed", "EMBED_RESOURCE"} // C++26
};

vector<pair<string, string>> delimiters = {
    {"{", "LEFT_CURLY_BRACE"},
    {"}", "RIGHT_CURLY_BRACE"},
    {"(", "LEFT_PARENTHESIS"},
    {")", "RIGHT_PARENTHESIS"},
    {"[", "LEFT_SQUARE_BRACKET"},
    {"]", "RIGHT_SQUARE_BRACKET"},
    {";", "SEMICOLON"},
    {",", "COMMA"},
    {":", "COLON"},
    {"::", "SCOPE_RESOLUTION"},
    {".", "DOT"},
    {"->", "ARROW_OPERATOR"},
    {"...", "ELLIPSIS"},
    {"#", "HASH"},
    {"##", "DOUBLE_HASH"},
    {"@", "AT_SYMBOL"} 
};

void define_tokens(){
	TOKENS["KEYWORD"] = keywords ;
	TOKENS["OPERATOR"] = operators ; 
	TOKENS["PREPROCESSORS"] = preprocessors ; 
	TOKENS["DELIMETER"] = delimiters ;	
}


    