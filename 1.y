%{
#define YYDEBUG 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *yyin;
int yylex(void);
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    exit(1);
}
%}

/* Token declarations */

%token PREPROCESSOR
%token IDENTIFIER NUMBER CHAR_LITERAL STRING_LITERAL TYPEDEF_NAME
%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token VOID CHAR SHORT INT LONG FLOAT DOUBLE SIGNED UNSIGNED BOOL
%token STRUCT UNION ENUM
%token CONST RESTRICT VOLATILE
%token INLINE NORETURN ALIGNAS SIZEOF ELLIPSIS
%token LPAREN RPAREN LSQBRACKET RSQBRACKET LBRACE RBRACE
%token SEMICOLON COMMA COLON DOT ARROW QUESTION
%token ASSIGN PLUS_ASSIGN MINUS_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token SHIFT_LEFT_ASSIGN SHIFT_RIGHT_ASSIGN BIT_AND_ASSIGN BIT_XOR_ASSIGN BIT_OR_ASSIGN
%token OR AND BIT_OR BIT_AND BIT_XOR XOR NOT BIT_NOT
%token EQUAL NOT_EQUAL LESS GREATER LESS_EQUAL GREATER_EQUAL
%token SHIFT_LEFT SHIFT_RIGHT PLUS MINUS MULTIPLY DIVIDE MODULO
%token IF ELSE WHILE GOTO BREAK CONTINUE FOR SWITCH CASE DEFAULT DO
%token INCREMENT DECREMENT
%token RETURN
%define parse.error verbose
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%


translation_unit
    : external_declaration
    | translation_unit external_declaration
    ;

external_declaration
    : function_definition
    | declaration
    | PREPROCESSOR
    ;

/* Helper lists */
declaration_list
    : declaration
    | declaration_list declaration
    ;

block_item
    : declaration
    | statement
    ;

/* Type qualifiers */
type_qualifier_list
    : type_qualifier
    | type_qualifier_list type_qualifier
    ;

/* Compound blocks */
compound_statement
    : LBRACE RBRACE
    | LBRACE non_empty_block_item_list RBRACE
    ;

non_empty_block_item_list
    : block_item
    | non_empty_block_item_list block_item
    ;

/* Declarations & definitions */
function_definition
    : declaration_specifiers declarator declaration_list compound_statement
    | declaration_specifiers declarator compound_statement
    ;

declaration
    : declaration_specifiers SEMICOLON
    | declaration_specifiers init_declarator_list SEMICOLON
    ;

init_declarator_list
    : init_declarator
    | init_declarator_list COMMA init_declarator
    ;

init_declarator
    : declarator ASSIGN initializer
    | declarator
    ;

declaration_specifiers
    : storage_class_specifier declaration_specifiers
    | storage_class_specifier
    | type_specifier declaration_specifiers
    | type_specifier
    | type_qualifier declaration_specifiers
    | type_qualifier
    | function_specifier declaration_specifiers
    | function_specifier
    | alignment_specifier declaration_specifiers
    | alignment_specifier
    ;

storage_class_specifier
    : TYPEDEF | EXTERN | STATIC | AUTO | REGISTER
    ;

type_specifier
    : VOID | CHAR | SHORT | INT | LONG | FLOAT | DOUBLE | SIGNED | UNSIGNED | BOOL
    | struct_or_union_specifier | enum_specifier | TYPEDEF_NAME 
    ;

type_qualifier
    : CONST | RESTRICT | VOLATILE
    ;

function_specifier
    : INLINE | NORETURN
    ;

alignment_specifier
    : ALIGNAS LPAREN type_name RPAREN
    ;

/* Declarator rules */
declarator
    : pointer direct_declarator
    | direct_declarator
    ;

pointer
    : MULTIPLY
    | MULTIPLY type_qualifier_list
    | MULTIPLY pointer
    | MULTIPLY type_qualifier_list pointer
    ;

direct_declarator
    : IDENTIFIER
    | LPAREN declarator RPAREN
    | direct_declarator LSQBRACKET assignment_expression RSQBRACKET
    | direct_declarator LPAREN RPAREN
    | direct_declarator LPAREN argument_expression_list RPAREN
    | direct_declarator LPAREN parameter_type_list RPAREN
    ;

/* Struct/union/enum */
struct_or_union_specifier
    : STRUCT LBRACE struct_declaration_list RBRACE
    | STRUCT IDENTIFIER LBRACE struct_declaration_list RBRACE
    | STRUCT IDENTIFIER
    | UNION LBRACE struct_declaration_list RBRACE
    | UNION IDENTIFIER LBRACE struct_declaration_list RBRACE
    | UNION IDENTIFIER
    ;

struct_declaration_list
    : struct_declaration
    | struct_declaration_list struct_declaration
    ;

struct_declaration
    : specifier_qualifier_list SEMICOLON
    | specifier_qualifier_list struct_declarator_list SEMICOLON
    ;

specifier_qualifier_list
    : type_specifier
    | type_specifier specifier_qualifier_list
    | type_qualifier
    | type_qualifier specifier_qualifier_list
    ;

struct_declarator_list
    : struct_declarator
    | struct_declarator_list COMMA struct_declarator
    ;

struct_declarator
    : declarator
    | declarator COLON constant_expression
    | COLON constant_expression
    ;

enum_specifier
    : ENUM LBRACE enumerator_list RBRACE
    | ENUM IDENTIFIER LBRACE enumerator_list RBRACE
    | ENUM IDENTIFIER
    ;

enumerator_list
    : enumeration_constant
    | enumerator_list COMMA enumeration_constant
    ;

enumeration_constant
    : IDENTIFIER
    ;

/* Type name */
type_name
    : specifier_qualifier_list
    | specifier_qualifier_list abstract_declarator
    ;

/* Abstract declarators */
abstract_declarator
    : pointer
    | pointer direct_abstract_declarator
    | direct_abstract_declarator
    ;

direct_abstract_declarator
    : LPAREN abstract_declarator RPAREN
    | LSQBRACKET RSQBRACKET
    | LSQBRACKET type_qualifier_list assignment_expression RSQBRACKET
    | direct_abstract_declarator LSQBRACKET assignment_expression RSQBRACKET
    | LPAREN parameter_type_list RPAREN
    | direct_abstract_declarator LPAREN parameter_type_list RPAREN
    ;

parameter_type_list
    : parameter_list
    | parameter_list COMMA ELLIPSIS
    ;

parameter_list
    : parameter_declaration
    | parameter_list COMMA parameter_declaration
    ;

parameter_declaration
    : declaration_specifiers declarator
    | declaration_specifiers abstract_declarator
    | declaration_specifiers
    ;

/* Initializers */
initializer
    : assignment_expression
    | LBRACE initializer_list RBRACE
    | LBRACE initializer_list COMMA RBRACE
    ;

initializer_list
    : initializer
    | initializer_list COMMA initializer
    ;

/* Statements */
statement
    : expression_statement
    | RETURN expression SEMICOLON
    | iteration_statement
    | conditional_statement
    | compound_statement
    | BREAK SEMICOLON
    | CONTINUE SEMICOLON
    ;

iteration_statement
    : DO statement WHILE LPAREN expression RPAREN SEMICOLON
    | WHILE LPAREN expression RPAREN statement
    | FOR LPAREN expression_statement expression_statement expression_statement RPAREN statement
    ;

conditional_statement 
    : IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE
    | IF LPAREN expression RPAREN statement ELSE statement
    ;

expression_statement
    : SEMICOLON
    | expression SEMICOLON
    ;

/* Expressions */
primary_expression
    : IDENTIFIER
    | NUMBER
    | CHAR_LITERAL
    | STRING_LITERAL
    | LPAREN expression RPAREN
    ;

postfix_expression
    : primary_expression
    | postfix_expression LSQBRACKET expression RSQBRACKET
    | postfix_expression LPAREN RPAREN
    | postfix_expression LPAREN argument_expression_list RPAREN
    | postfix_expression DOT IDENTIFIER
    | postfix_expression ARROW IDENTIFIER
    | postfix_expression INCREMENT
    | postfix_expression DECREMENT
    ;

argument_expression_list
    : assignment_expression
    | argument_expression_list COMMA assignment_expression
    ;

unary_expression
    : postfix_expression
    | INCREMENT unary_expression
    | DECREMENT unary_expression
    | unary_operator cast_expression
    | SIZEOF unary_expression
    | SIZEOF LPAREN type_name RPAREN
    ;

unary_operator
    : BIT_AND | MULTIPLY | PLUS | MINUS | BIT_NOT | NOT
    ;

cast_expression
    : unary_expression
    | LPAREN type_name RPAREN cast_expression
    ;

multiplicative_expression
    : cast_expression
    | multiplicative_expression MULTIPLY cast_expression
    | multiplicative_expression DIVIDE cast_expression
    | multiplicative_expression MODULO cast_expression
    ;

additive_expression
    : multiplicative_expression
    | additive_expression PLUS multiplicative_expression
    | additive_expression MINUS multiplicative_expression
    ;

shift_expression
    : additive_expression
    | shift_expression SHIFT_LEFT additive_expression
    | shift_expression SHIFT_RIGHT additive_expression
    ;

relational_expression
    : shift_expression
    | relational_expression LESS shift_expression
    | relational_expression GREATER shift_expression
    | relational_expression LESS_EQUAL shift_expression
    | relational_expression GREATER_EQUAL shift_expression
    ;

equality_expression
    : relational_expression
    | equality_expression EQUAL relational_expression
    | equality_expression NOT_EQUAL relational_expression
    ;

and_expression
    : equality_expression
    | and_expression BIT_AND equality_expression
    ;

exclusive_or_expression
    : and_expression
    | exclusive_or_expression BIT_XOR and_expression
    ;

inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression BIT_OR exclusive_or_expression
    ;

logical_and_expression
    : inclusive_or_expression
    | logical_and_expression AND inclusive_or_expression
    ;

logical_or_expression
    : logical_and_expression
    | logical_or_expression OR logical_and_expression
    ;

conditional_expression
    : logical_or_expression
    | logical_or_expression QUESTION expression COLON conditional_expression
    ;

assignment_expression
    : conditional_expression
    | unary_expression assignment_operator assignment_expression
    ;

assignment_operator
    : ASSIGN | PLUS_ASSIGN | MINUS_ASSIGN | MUL_ASSIGN | DIV_ASSIGN
    | MOD_ASSIGN | SHIFT_LEFT_ASSIGN | SHIFT_RIGHT_ASSIGN
    | BIT_AND_ASSIGN | BIT_XOR_ASSIGN | BIT_OR_ASSIGN
    ;

expression
    : assignment_expression
    | expression COMMA assignment_expression
    ;

constant_expression
    : conditional_expression
    ;


%%

int yylex(void) {
    char tok[64];
    if (fscanf(yyin, "%63s", tok) != 1) return 0;
    /* Map token text to token codes */
    #define MATCH(t) if (strcmp(tok,#t)==0) return t;
    MATCH(PREPROCESSOR)
    MATCH(IDENTIFIER) MATCH(NUMBER) MATCH(CHAR_LITERAL) MATCH(STRING_LITERAL)
    MATCH(TYPEDEF_NAME) MATCH(TYPEDEF) MATCH(EXTERN) MATCH(STATIC) MATCH(AUTO) MATCH(REGISTER)
    MATCH(VOID) MATCH(CHAR) MATCH(SHORT) MATCH(INT) MATCH(LONG) MATCH(FLOAT) MATCH(DOUBLE) MATCH(SIGNED) MATCH(UNSIGNED) MATCH(BOOL)
    MATCH(STRUCT) MATCH(UNION) MATCH(ENUM) MATCH(CONST) MATCH(RESTRICT) MATCH(VOLATILE)
    MATCH(INLINE) MATCH(NORETURN) MATCH(ALIGNAS) MATCH(SIZEOF) MATCH(ELLIPSIS) MATCH(WHILE) MATCH(FOR) MATCH(IF) MATCH(ELSE)
    MATCH(LPAREN) MATCH(RPAREN) MATCH(LSQBRACKET) MATCH(RSQBRACKET) MATCH(LBRACE) MATCH(RBRACE)
    MATCH(SEMICOLON) MATCH(COMMA) MATCH(COLON) MATCH(DOT) MATCH(ARROW) MATCH(QUESTION)
    MATCH(ASSIGN) MATCH(PLUS_ASSIGN) MATCH(MINUS_ASSIGN) MATCH(MUL_ASSIGN) MATCH(DIV_ASSIGN) MATCH(MOD_ASSIGN)
    MATCH(SHIFT_LEFT_ASSIGN) MATCH(SHIFT_RIGHT_ASSIGN) MATCH(BIT_AND_ASSIGN) MATCH(BIT_XOR_ASSIGN) MATCH(BIT_OR_ASSIGN)
    MATCH(OR) MATCH(AND) MATCH(BIT_OR) MATCH(BIT_AND) MATCH(XOR) MATCH(NOT) MATCH(BIT_NOT)
    MATCH(EQUAL) MATCH(NOT_EQUAL) MATCH(LESS) MATCH(GREATER) MATCH(LESS_EQUAL) MATCH(GREATER_EQUAL)
    MATCH(SHIFT_LEFT) MATCH(SHIFT_RIGHT)
    MATCH(PLUS) MATCH(MINUS) MATCH(MULTIPLY) MATCH(DIVIDE) MATCH(MODULO) MATCH(BREAK) MATCH(CONTINUE)
    MATCH(INCREMENT) MATCH(DECREMENT) MATCH(RETURN)
    fprintf(stderr, "Unknown token: %s\n", tok);
    exit(1);
}
extern int yydebug;
int main(int argc, char **argv) {
    //yydebug = 1;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s tokens.txt\n", argv[0]);
        return 1;
    }
    yyin = fopen(argv[1], "r");
    int tok, i;
    rewind(yyin);
    /* you’d need your own tokname() here or just print tok */
    for (i = 0; i < 20 && (tok = yylex()); ++i)
        printf("tok[%d] = %d\n", i, tok);
    rewind(yyin);
    return yyparse() == 0 ? 0 : 1;
}
