#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
unordered_map<string, vector<pair<string, string>>> TOKENS;

vector<pair<string, string>> keywords = {
    {"auto", "AUTO"},{"break", "BREAK"},{"case", "CASE"},{"char", "CHAR"},{"const", "CONST"},{"continue", "CONTINUE"},
    {"default", "DEFAULT"},{"do", "DO"},{"double", "DOUBLE"},{"else", "ELSE"},{"enum", "ENUM"},{"extern", "EXTERN"},
    {"float", "FLOAT"},{"for", "FOR"},{"_Alignof", "ALIGNOF"},{"_Atomic", "ATOMIC"},
    {"goto", "GOTO"},{"if", "IF"},{"inline", "INLINE"},{"int", "INT"},{"long", "LONG"},{"register", "REGISTER"},
    {"restrict", "RESTRICT"},{"return", "RETURN"},{"short", "SHORT"},{"signed", "SIGNED"},{"sizeof", "SIZEOF"},
    {"static", "STATIC"},{"struct", "STRUCT"},{"switch", "SWITCH"},{"while", "WHILE"},{"_Alignas", "ALIGNAS"},
    {"typedef", "TYPEDEF"},{"union", "UNION"},{"unsigned", "UNSIGNED"},{"void", "VOID"},{"volatile", "VOLATILE"},
    {"_Bool", "BOOL"},{"_Complex", "COMPLEX"},{"_Generic", "GENERIC"},{"_Imaginary", "IMAGINARY"},
    {"_Noreturn", "NORETURN"},{"_Static_assert", "STATIC_ASSERT"},{"_Thread_local", "THREAD_LOCAL"}
};


vector<pair<string, string>> operators = {
    {"+", "ADD"}, {"-", "SUB"}, {"*", "MUL"}, {"/", "DIV"}, {"%", "MOD"},
    {"++", "INR"}, {"--", "DCR"}, {"==", "EQUAL"}, {"!=", "NOT_EQUAL"},
    {">", "GREATER"}, {"<", "LESS"}, {">=", "GREATER_EQUAL"}, {"<=", "LESS_EQUAL"},
    {"&&", "LOGICAL_AND"}, {"||", "LOGICAL_OR"}, {"!", "LOGICAL_NOT"}, {"&", "BIT_AND"},
    {"|", "BIT_OR"}, {"^", "BIT_XOR"}, {"~", "BIT_NOT"}, {"<<", "BIT_LEFT_SHIFT"},
    {">>", "BIT_RIGHT_SHIFT"}, {"+=", "ADD_ASSIGN"}, {"-=", "SUB_ASSIGN"}, {"*=", "MUL_ASSIGN"},
    {"/=", "DIV_ASSIGN"}, {"%=", "MOD_ASSIGN"}, {"&=", "BITWISE_AND_ASSIGN"},
    {"|=", "BIT_OR_ASSIGN"}, {"^=", "BIT_XOR_ASSIGN"}, {"<<=", "LEFT_SHIFT_ASSIGN"},
    {">>=", "RIGHT_SHIFT_ASSIGN"}, {"=", "ASSIGN"}, {"?", "TERNARY_CONDITIONAL"}, {":", "TERNARY_SEPARATOR"},
    {"->", "ARROW"}
};

vector<pair<string, string>> preprocessors = {
    {"#include", "INCLUDE_HEADER"},{"#define", "DEFINE_MACRO"},{"#undef", "UNDEFINE_MACRO"},
    {"#if", "IF_CONDITIONAL"},{"#ifdef", "IF_DEFINED"},{"#ifndef", "IF_NOT_DEFINED"},{"#elif"," ELIF_CONDITIONAL"},
    {"#elifdef", "ELIFDEF_CONDITIONAL"}, {"#elifndef", "ELIFNDEF_CONDITIONAL"}, {"#else", "ELSE_CONDITIONAL"},
    {"#endif", "END_IF_CONDITIONAL"},{"#error", "ERROR_DIRECTIVE"},{"#warning", "WARNING_DIRECTIVE"},  
    {"#pragma", "PRAGMA_DIRECTIVE"}, {"#line", "LINE_DIRECTIVE"}, {"#embed","EMBED_DIRECTIVE"}       
};

vector<pair<string, string>> delimiters = {
    {"{", "L_CURLY"}, {"}", "R_CURLY"}, {"(", "L_PAREN"}, {")", "R_PAREN"},
    {"[", "L_SQUARE"}, {"]", "R_SQUARE"}, {";", "SEMI"}, {",", "COMMA"},
    {":", "COLON"}, {"::", "SCOPE_RESOLUTION"}, {".", "DOT"},
    {"...", "ELLIPSIS"}, {"#", "HASH"}, {"##", "HASH_HASH"}, {"@", "AT_SYMBOL"}
};

vector<pair<string, string>> comment = {{"//","X"},{"/*","X"}} ;


void define_tokens() {
    TOKENS["KEYWORD"] = keywords;
    TOKENS["OPERATOR"] = operators;
    TOKENS["PREPROCESSORS"] = preprocessors;
    TOKENS["DELIMITER"] = delimiters;
    TOKENS["CT"] = comment ;
}

void store_tokens_to_file(const string& filename) {
    ofstream outFile(filename);
    
    if (!outFile) {
        cerr << "Error: Could not open file for writing." << endl;
        return;
    }

    for (const auto& category : TOKENS) {
        outFile << "\\ \n" << category.first << "\n"; // Write category name

        for (const auto& token : category.second) {
            outFile << token.first << " " << token.second << "\n";
        }
    }

    outFile << "\\"  ; // Separate categories

    outFile.close();
    cout << "Tokens stored successfully in " << filename << endl;
}

int main() {
    define_tokens();
    store_tokens_to_file("CTokens_map.txt");
    return 0;
}
