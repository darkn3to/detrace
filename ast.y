%{
#define YYDEBUG 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define makelist(x) create_ast_node(AST_LIST, 1, x)
#define makelist_leaf(x) create_leaf_node(AST_LIST, x)
#define append_list(x1, x2) ((x1 && x2) ? create_ast_node(AST_LIST, 2, x1, x2) : (x1) ? x1 : (struct ASTNode *)NULL)

typedef enum {
    AST_FUNCTION_DEF,
    AST_PARAM_LIST,
    AST_PARAM,
    AST_DECLARATION,
    AST_INIT_DECLARATOR,
    AST_ASSIGNMENT,
    AST_PREPROCESSOR,
    AST_PLUS, AST_MINUS, AST_MUL, AST_DIV, AST_MOD,
    AST_LT, AST_GT, AST_LE, AST_GE,
    AST_FUNCTION_CALL,
    AST_IDENTIFIER,
    AST_LITERAL,
    AST_NUMBER,
    AST_RETURN,
    AST_IF,
    AST_ITERATION_STMT,
    AST_COMPOUND_STMT,
    AST_ARRAY_ACCESS,
    AST_STRUCT_DECL,
    AST_TYPEDEF,
    AST_INIT_LIST,
    AST_POINTER_DEREF,
    AST_ADDRESS_OF,
    AST_BREAK,
    AST_CONTNUE,
    AST_SIZEOF,
    AST_POINTER,
    AST_TYPE_QUALIFIER,
    AST_TYPE_SPECIFIER,
    AST_FUNCTION_SPECIFIER,
    AST_TRANS,
    AST_LIST,
    AST_BLOCK,
    AST_ALIGNMENT_SPECIFIER,
    AST_STORAGE_CLASS_SPECIFIER, 
    AST_DECLARATOR,
    AST_ENUM,
    AST_TYPE_NAME, 
    AST_CONTINUE
} ASTNodeType;

struct ASTNode {
    ASTNodeType type;
    int val;
    char *value;
    int child_count;
    struct ASTNode **children;
} ASTNode;

struct ASTNode *create_ast_node(ASTNodeType type, int n, ...);
struct ASTNode *create_leaf_node(ASTNodeType type, const char *val);
void print_ast(struct ASTNode *node, int depth);
const char *ast_node_type_to_string(ASTNodeType type);

struct ASTNode *root;

FILE *yyin;
int yylex(void);
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    exit(1);
}
%}

%union {
    struct ASTNode *node;
    int val;
    struct ASTNodeList *list;
    char *sval;
}

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

%type <node> translation_unit external_declaration function_definition declaration declaration_list statement block_item type_qualifier_list compound_statement non_empty_block_item_list declaration_specifiers declarator init_declarator_list init_declarator initializer alignment_specifier pointer type_name direct_declarator storage_class_specifier type_specifier type_specifier_spec type_qualifier function_specifier struct_or_union_specifier enum_specifier specifier_qualifier_list struct_declaration_list struct_declaration struct_declarator_list struct_declarator parameter_type_list parameter_list parameter_declaration abstract_declarator direct_abstract_declarator initializer_list expression_statement iteration_statement conditional_statement expression assignment_expression constant_expression primary_expression postfix_expression argument_expression_list unary_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression conditional_expression enumeration_constant enumerator_list unary_operator assignment_operator expression_opt

%type <sval> PREPROCESSOR CONST RESTRICT VOLATILE TYPEDEF EXTERN STATIC AUTO REGISTER VOID CHAR SHORT INT LONG FLOAT DOUBLE SIGNED UNSIGNED BOOL INLINE NORETURN TYPEDEF_NAME IDENTIFIER NUMBER CHAR_LITERAL STRING_LITERAL

%%


translation_unit
    : external_declaration { root = $1; $$ = $1; }
    | translation_unit external_declaration { $$ = create_ast_node(AST_TRANS, 2, $1, $2);
    root = $$; }
    ;

external_declaration
    : function_definition { $$ = $1; }
    | declaration { $$ = $1; }
    | PREPROCESSOR { $$ = create_leaf_node(AST_PREPROCESSOR, $1); }
    ;

/* Helper lists */
declaration_list
    : declaration { $$ = makelist($1); }
    | declaration_list declaration { $$ = append_list($1, $2); }
    ;

block_item
    : declaration { $$ = $1; }
    | statement { $$ = $1; }
    ;

/* Type qualifiers */
type_qualifier_list
    : type_qualifier { $$ = makelist($1); }
    | type_qualifier_list type_qualifier { $$ = append_list($1, $2); }
    ;

/* Compound blocks */
compound_statement
    : LBRACE RBRACE { $$ = create_ast_node(AST_COMPOUND_STMT, 1, NULL); }
    | LBRACE non_empty_block_item_list RBRACE { $$ = create_ast_node(AST_COMPOUND_STMT, 1, $2); }
    ;

non_empty_block_item_list
    : block_item { $$ = makelist($1); }
    | non_empty_block_item_list block_item { $$ = append_list($1, $2); } 
    ;

/* Declarations & definitions */
function_definition
    : declaration_specifiers declarator declaration_list compound_statement { $$ = create_ast_node(AST_FUNCTION_DEF, 4, $1, $2, $3, $4); } 
    | declaration_specifiers declarator compound_statement { $$ = create_ast_node(AST_FUNCTION_DEF, 3, $1, $2, $3); }
    ;

declaration
    : declaration_specifiers SEMICOLON { $$ = create_ast_node(AST_DECLARATION, 1, $1); }
    | declaration_specifiers init_declarator_list SEMICOLON { $$ = create_ast_node(AST_DECLARATION, 2, $1, $2); }
    ;

init_declarator_list
    : init_declarator { $$ = makelist($1); }
    | init_declarator_list COMMA init_declarator { $$ = append_list($1, $3); }
    ;

init_declarator 
    : declarator ASSIGN initializer { $$ = create_ast_node(AST_INIT_DECLARATOR, 2, $1, $3); }
    | declarator { $$ = $1; }
    ;

declaration_specifiers
    : storage_class_specifier { $$ = makelist($1); }
    | type_specifier { $$ = makelist($1); }
    | type_qualifier { $$ = makelist($1); }
    | function_specifier { $$ = makelist($1); }
    | alignment_specifier { $$ = makelist($1); }
    | declaration_specifiers storage_class_specifier { $$ = append_list($1, $2); }
    | declaration_specifiers type_specifier { $$ = append_list($1, $2); }
    | declaration_specifiers type_qualifier { $$ = append_list($1, $2); }
    | declaration_specifiers function_specifier { $$ = append_list($1, $2); }
    | declaration_specifiers alignment_specifier { $$ = append_list($1, $2); }
    ;

storage_class_specifier
    : TYPEDEF { $$ = create_leaf_node(AST_STORAGE_CLASS_SPECIFIER, "typedef"); }
    | EXTERN { $$ = create_leaf_node(AST_STORAGE_CLASS_SPECIFIER, "extern"); }
    | STATIC { $$ = create_leaf_node(AST_STORAGE_CLASS_SPECIFIER, "static"); }
    | AUTO { $$ = create_leaf_node(AST_STORAGE_CLASS_SPECIFIER, "auto"); }
    | REGISTER { $$ = create_leaf_node(AST_STORAGE_CLASS_SPECIFIER, "register"); }
    ;

type_specifier
    : VOID { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "void"); }
    | CHAR { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "char"); }
    | SHORT { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "short"); }
    | INT { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "int"); }
    | LONG { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "long"); }
    | FLOAT { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "float"); }
    | DOUBLE { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "double"); }
    | SIGNED { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "signed"); }
    | UNSIGNED { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "unsigned"); }
    | BOOL { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "bool"); }
    | struct_or_union_specifier { $$ = $1; }
    | enum_specifier { $$ = $1; }
    | TYPEDEF_NAME { $$ = create_leaf_node(AST_TYPE_SPECIFIER, $1); }
    ;

type_specifier_spec
    : VOID { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "void"); }
    | CHAR { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "char"); }
    | SHORT { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "short"); }
    | INT { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "int"); }
    | LONG { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "long"); }
    | FLOAT { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "float"); }
    | DOUBLE { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "double"); }
    | SIGNED { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "signed"); }
    | UNSIGNED { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "unsigned"); }
    | BOOL { $$ = create_leaf_node(AST_TYPE_SPECIFIER, "bool"); }
    | struct_or_union_specifier { $$ = $1; }
    | enum_specifier { $$ = $1; }
    | TYPEDEF_NAME { $$ = create_leaf_node(AST_TYPE_SPECIFIER, $1); }
    | /* */ { $$ = NULL; }
    ;

type_qualifier
    : CONST { $$ = create_leaf_node(AST_TYPE_QUALIFIER, "const"); }
    | RESTRICT { $$ = create_leaf_node(AST_TYPE_QUALIFIER, "restrict"); }
    | VOLATILE { $$ = create_leaf_node(AST_TYPE_QUALIFIER, "volatile"); }
    ;

function_specifier
    : INLINE { $$ = create_leaf_node(AST_FUNCTION_SPECIFIER, "inline"); }
    | NORETURN { $$ = create_leaf_node(AST_FUNCTION_SPECIFIER, "noreturn"); }
    ;

alignment_specifier
    : ALIGNAS LPAREN type_name RPAREN { $$ = create_ast_node(AST_ALIGNMENT_SPECIFIER, 1, $3); }
    ;

/* Declarator rules */
declarator
    : pointer direct_declarator { $$ = create_ast_node(AST_DECLARATOR, 2, $1, $2); }
    | direct_declarator { $$ = create_ast_node(AST_DECLARATOR, 1, $1); }
    ;

pointer
    : MULTIPLY { $$ = create_ast_node(AST_POINTER, 1, NULL);}
    | MULTIPLY type_qualifier_list { $$ = create_ast_node(AST_POINTER, 1, $2);}
    | MULTIPLY pointer { $$ = create_ast_node(AST_POINTER, 1, $2); }
    | MULTIPLY type_qualifier_list pointer { $$ = create_ast_node(AST_POINTER, 2, $2, $3); }
    ;

direct_declarator
    : IDENTIFIER { $$ = create_leaf_node(AST_IDENTIFIER, $1); }
    | LPAREN declarator RPAREN { $$ = $2; }
    | direct_declarator LSQBRACKET RSQBRACKET { $$ = create_ast_node(AST_ARRAY_ACCESS, 0); }
    | direct_declarator LSQBRACKET assignment_expression RSQBRACKET { $$ = create_ast_node(AST_ARRAY_ACCESS, 2, $1, $3); }
    | direct_declarator LPAREN RPAREN { $$ = create_ast_node(AST_FUNCTION_CALL, 1, $1); }
    | direct_declarator LPAREN argument_expression_list RPAREN { $$ = create_ast_node(AST_FUNCTION_CALL, 2, $1, $3); }
    | direct_declarator LPAREN parameter_type_list RPAREN { $$ = create_ast_node(AST_FUNCTION_CALL, 2, $1, $3); }
    ;

struct_or_union_specifier
    : STRUCT LBRACE struct_declaration_list RBRACE { $$ = create_ast_node(AST_STRUCT_DECL, 1, $3); }
    | STRUCT IDENTIFIER LBRACE struct_declaration_list RBRACE { $$ = create_ast_node(AST_STRUCT_DECL, 2, create_leaf_node(AST_IDENTIFIER, $2), $4); }
    | STRUCT IDENTIFIER { $$ = create_ast_node(AST_STRUCT_DECL, 1, create_leaf_node(AST_IDENTIFIER, $2)); }
    | UNION LBRACE struct_declaration_list RBRACE { $$ = create_ast_node(AST_STRUCT_DECL, 1, $3); }
    | UNION IDENTIFIER LBRACE struct_declaration_list RBRACE { $$ = create_ast_node(AST_STRUCT_DECL, 2, create_leaf_node(AST_IDENTIFIER, $2), $4); }
    | UNION IDENTIFIER { $$ = create_ast_node(AST_STRUCT_DECL, 1, create_leaf_node(AST_IDENTIFIER, $2)); }
    ;

struct_declaration_list
    : struct_declaration { $$ = makelist($1); }
    | struct_declaration_list struct_declaration { $$ = append_list($1, $2); }
    ;

struct_declaration
    : specifier_qualifier_list SEMICOLON { $$ = $1; }
    | specifier_qualifier_list struct_declarator_list SEMICOLON { $$ = create_ast_node(AST_DECLARATION, 2, $1, $2); }
    ;

specifier_qualifier_list
    : type_specifier { $$ = $1; }
    | type_specifier specifier_qualifier_list { $$ = append_list($1, $2); }
    | type_qualifier { $$ = $1; }
    | type_qualifier specifier_qualifier_list { $$ = append_list($1, $2); }
    ;

struct_declarator_list
    : struct_declarator { $$ = makelist($1); }
    | struct_declarator_list COMMA struct_declarator { $$ = append_list($1, $3); }
    ;

struct_declarator
    : declarator { $$ = $1; }
    | declarator COLON constant_expression { $$ = create_ast_node(AST_DECLARATOR, 2, $1, $3); }
    | COLON constant_expression { $$ = create_ast_node(AST_DECLARATOR, 1, $2); }
    ;

enum_specifier
    : ENUM LBRACE enumerator_list RBRACE { $$ = create_ast_node(AST_ENUM, 1, $3); }
    | ENUM IDENTIFIER LBRACE enumerator_list RBRACE { $$ = create_ast_node(AST_ENUM, 2, create_leaf_node(AST_IDENTIFIER, $2), $4); }
    | ENUM IDENTIFIER { $$ = create_ast_node(AST_ENUM, 1, create_leaf_node(AST_IDENTIFIER, $2)); }
    ;

enumerator_list
    : enumeration_constant { $$ = makelist($1); }
    | enumerator_list COMMA enumeration_constant { $$ = append_list($1, $3); }
    ;

enumeration_constant
    : IDENTIFIER { $$ = create_leaf_node(AST_IDENTIFIER, $1); }
    ;

type_name
    : specifier_qualifier_list { $$ = $1; }
    | specifier_qualifier_list abstract_declarator { $$ = create_ast_node(AST_TYPE_NAME, 2, $1, $2); }
    ;

abstract_declarator
    : pointer { $$ = $1; }
    | pointer direct_abstract_declarator { $$ = create_ast_node(AST_DECLARATOR, 2, $1, $2); }
    | direct_abstract_declarator { $$ = $1; }
    ;

direct_abstract_declarator
    : LPAREN abstract_declarator RPAREN { $$ = $2; }
    | LSQBRACKET RSQBRACKET { $$ = create_ast_node(AST_ARRAY_ACCESS, 0); }
    | LSQBRACKET type_qualifier_list assignment_expression RSQBRACKET { $$ = create_ast_node(AST_ARRAY_ACCESS, 2, $2, $3); }
    | direct_abstract_declarator LSQBRACKET assignment_expression RSQBRACKET { $$ = create_ast_node(AST_ARRAY_ACCESS, 2, $1, $3); }
    | LPAREN parameter_type_list RPAREN { $$ = create_ast_node(AST_FUNCTION_CALL, 1, $2); }
    | direct_abstract_declarator LPAREN parameter_type_list RPAREN { $$ = create_ast_node(AST_FUNCTION_CALL, 2, $1, $3); }
    ;

parameter_type_list
    : parameter_list { $$ = $1; }
    | parameter_list COMMA ELLIPSIS { $$ = create_ast_node(AST_PARAM_LIST, 2, $1, create_leaf_node(AST_IDENTIFIER, "...")); }
    ;

parameter_list
    : parameter_declaration { $$ = makelist($1); }
    | parameter_list COMMA parameter_declaration { $$ = append_list($1, $3); }
    ;

parameter_declaration
    : declaration_specifiers declarator { $$ = create_ast_node(AST_PARAM, 2, $1, $2); }
    | declaration_specifiers abstract_declarator { $$ = create_ast_node(AST_PARAM, 2, $1, $2); }
    | declaration_specifiers { $$ = $1; }
    ;

initializer
    : assignment_expression { $$ = $1; }
    | LBRACE initializer_list RBRACE { $$ = create_ast_node(AST_INIT_LIST, 1, $2); }
    | LBRACE initializer_list COMMA RBRACE { $$ = create_ast_node(AST_INIT_LIST, 1, $2); }
    ;

initializer_list
    : initializer { $$ = makelist($1); }
    | initializer_list COMMA initializer { $$ = append_list($1, $3); }
    ;

statement
    : expression_statement { $$ = $1; }
    | RETURN expression SEMICOLON { $$ = create_ast_node(AST_RETURN, 1, $2); }
    | RETURN SEMICOLON { $$ = create_ast_node(AST_RETURN, 0); }
    | iteration_statement { $$ = $1; }
    | conditional_statement { $$ = $1; }
    | compound_statement { $$ = $1; }
    | BREAK SEMICOLON { $$ = create_ast_node(AST_BREAK, 0); }
    | CONTINUE SEMICOLON { $$ = create_ast_node(AST_CONTINUE, 0); }
    ;

iteration_statement
    : DO statement WHILE LPAREN expression RPAREN SEMICOLON { $$ = create_ast_node(AST_ITERATION_STMT, 2, $2, $5); }
    | WHILE LPAREN expression RPAREN statement { $$ = create_ast_node(AST_ITERATION_STMT, 2, $3, $5); }
    | FOR LPAREN type_specifier_spec expression_statement expression_statement expression_opt RPAREN statement { $$ = create_ast_node(AST_ITERATION_STMT, 4, $4, $5, $6, $8); }
    ;

conditional_statement
    : IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE { $$ = create_ast_node(AST_IF, 2, $3, $5); }
    | IF LPAREN expression RPAREN statement ELSE statement { $$ = create_ast_node(AST_IF, 3, $3, $5, $7); }
    ;

expression_opt 
    : expression { $$ = $1; }
    | /* empty */ { $$ = NULL; }
    ;

expression_statement
    : SEMICOLON { $$ = NULL; }
    | expression SEMICOLON { $$ = $1; }
    ;

primary_expression
    : IDENTIFIER { $$ = create_leaf_node(AST_IDENTIFIER, $1); }
    | NUMBER { $$ = create_leaf_node(AST_NUMBER, $1); }
    | CHAR_LITERAL { $$ = create_leaf_node(AST_LITERAL, $1); }
    | STRING_LITERAL { $$ = create_leaf_node(AST_LITERAL, $1); }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

postfix_expression
    : primary_expression { $$ = $1; }
    | postfix_expression LSQBRACKET expression RSQBRACKET { $$ = create_ast_node(AST_ARRAY_ACCESS, 2, $1, $3); }
    | postfix_expression LPAREN RPAREN { $$ = create_ast_node(AST_FUNCTION_CALL, 1, $1); }
    | postfix_expression LPAREN argument_expression_list RPAREN { $$ = create_ast_node(AST_FUNCTION_CALL, 2, $1, $3); }
    | postfix_expression DOT IDENTIFIER { $$ = create_ast_node(AST_STRUCT_DECL, 2, $1, create_leaf_node(AST_IDENTIFIER, $3)); }
    | postfix_expression ARROW IDENTIFIER { $$ = create_ast_node(AST_POINTER_DEREF, 2, $1, create_leaf_node(AST_IDENTIFIER, $3)); }
    | postfix_expression INCREMENT { $$ = create_ast_node(AST_TRANS, 1, $1); }
    | postfix_expression DECREMENT { $$ = create_ast_node(AST_TRANS, 1, $1); }
    ;

argument_expression_list
    : assignment_expression { $$ = makelist($1); }
    | argument_expression_list COMMA assignment_expression { $$ = append_list($1, $3); }
    ;

unary_expression
    : postfix_expression { $$ = $1; }
    | INCREMENT unary_expression { $$ = create_ast_node(AST_TRANS, 1, $2); }
    | DECREMENT unary_expression { $$ = create_ast_node(AST_TRANS, 1, $2); }
    | unary_operator cast_expression { $$ = create_ast_node(AST_TRANS, 2, $1, $2); }
    | SIZEOF unary_expression { $$ = create_ast_node(AST_SIZEOF, 1, $2); }
    | SIZEOF LPAREN type_name RPAREN { $$ = create_ast_node(AST_SIZEOF, 1, $3); }
    ;

unary_operator
    : BIT_AND { $$ = create_leaf_node(AST_TRANS, "&"); }
    | MULTIPLY { $$ = create_leaf_node(AST_TRANS, "*"); }
    | PLUS { $$ = create_leaf_node(AST_TRANS, "+"); }
    | MINUS { $$ = create_leaf_node(AST_TRANS, "-"); }
    | BIT_NOT { $$ = create_leaf_node(AST_TRANS, "~"); }
    | NOT { $$ = create_leaf_node(AST_TRANS, "!"); }
    ;

cast_expression
    : unary_expression { $$ = $1; }
    | LPAREN type_name RPAREN cast_expression { $$ = create_ast_node(AST_TRANS, 2, $2, $4); }
    ;

multiplicative_expression
    : cast_expression { $$ = $1; }
    | multiplicative_expression MULTIPLY cast_expression { $$ = create_ast_node(AST_MUL, 2, $1, $3); }
    | multiplicative_expression DIVIDE cast_expression { $$ = create_ast_node(AST_DIV, 2, $1, $3); }
    | multiplicative_expression MODULO cast_expression { $$ = create_ast_node(AST_MOD, 2, $1, $3); }
    ;

additive_expression
    : multiplicative_expression { $$ = $1; }
    | additive_expression PLUS multiplicative_expression { $$ = create_ast_node(AST_PLUS, 2, $1, $3); }
    | additive_expression MINUS multiplicative_expression { $$ = create_ast_node(AST_MINUS, 2, $1, $3); }
    ;

shift_expression
    : additive_expression { $$ = $1; }
    | shift_expression SHIFT_LEFT additive_expression { $$ = create_ast_node(AST_TRANS, 2, $1, $3); }
    | shift_expression SHIFT_RIGHT additive_expression { $$ = create_ast_node(AST_TRANS, 2, $1, $3); }
    ;

relational_expression
    : shift_expression { $$ = $1; }
    | relational_expression LESS shift_expression { $$ = create_ast_node(AST_LT, 2, $1, $3); }
    | relational_expression GREATER shift_expression { $$ = create_ast_node(AST_GT, 2, $1, $3); }
    | relational_expression LESS_EQUAL shift_expression { $$ = create_ast_node(AST_LE, 2, $1, $3); }
    | relational_expression GREATER_EQUAL shift_expression { $$ = create_ast_node(AST_GE, 2, $1, $3); }
    ;

equality_expression
    : relational_expression { $$ = $1; }
    | equality_expression EQUAL relational_expression { $$ = create_ast_node(AST_TRANS, 2, $1, $3); }
    | equality_expression NOT_EQUAL relational_expression { $$ = create_ast_node(AST_TRANS, 2, $1, $3); }
    ;

and_expression
    : equality_expression { $$ = $1; }
    | and_expression BIT_AND equality_expression { $$ = create_ast_node(AST_TRANS, 2, $1, $3); }
    ;

exclusive_or_expression
    : and_expression { $$ = $1; }
    | exclusive_or_expression BIT_XOR and_expression { $$ = create_ast_node(AST_TRANS, 2, $1, $3); }
    ;

inclusive_or_expression
    : exclusive_or_expression { $$ = $1; }
    | inclusive_or_expression BIT_OR exclusive_or_expression { $$ = create_ast_node(AST_TRANS, 2, $1, $3); }
    ;

logical_and_expression
    : inclusive_or_expression { $$ = $1; }
    | logical_and_expression AND inclusive_or_expression { $$ = create_ast_node(AST_TRANS, 2, $1, $3); }
    ;

logical_or_expression
    : logical_and_expression { $$ = $1; }
    | logical_or_expression OR logical_and_expression { $$ = create_ast_node(AST_TRANS, 2, $1, $3); }
    ;

conditional_expression
    : logical_or_expression { $$ = $1; }
    | logical_or_expression QUESTION expression COLON conditional_expression { $$ = create_ast_node(AST_TRANS, 3, $1, $3, $5); }
    ;

assignment_expression
    : conditional_expression { $$ = $1; }
    | unary_expression assignment_operator assignment_expression { $$ = create_ast_node(AST_ASSIGNMENT, 2, $1, $3); }
    ;

assignment_operator
    : ASSIGN { $$ = create_leaf_node(AST_TRANS, "="); }
    | PLUS_ASSIGN { $$ = create_leaf_node(AST_TRANS, "+="); }
    | MINUS_ASSIGN { $$ = create_leaf_node(AST_TRANS, "-="); }
    | MUL_ASSIGN { $$ = create_leaf_node(AST_TRANS, "*="); }
    | DIV_ASSIGN { $$ = create_leaf_node(AST_TRANS, "/="); }
    | MOD_ASSIGN { $$ = create_leaf_node(AST_TRANS, "%="); }
    | SHIFT_LEFT_ASSIGN { $$ = create_leaf_node(AST_TRANS, "<<="); }
    | SHIFT_RIGHT_ASSIGN { $$ = create_leaf_node(AST_TRANS, ">>="); }
    | BIT_AND_ASSIGN { $$ = create_leaf_node(AST_TRANS, "&="); }
    | BIT_XOR_ASSIGN { $$ = create_leaf_node(AST_TRANS, "^="); }
    | BIT_OR_ASSIGN { $$ = create_leaf_node(AST_TRANS, "|="); }
    ;

expression
    : assignment_expression { $$ = $1; }
    | expression COMMA assignment_expression { $$ = create_ast_node(AST_TRANS, 2, $1, $3); }
    ;

constant_expression
    : conditional_expression { $$ = $1; }
    ;

%%


struct ASTNode *create_ast_node(ASTNodeType type, int n, ...) {
    va_list args;
    va_start(args, n);

    struct ASTNode *ret = malloc(sizeof *ret);
    ret->type        = type;
    ret->val         = 0;        // no integer payload
    ret->value       = NULL;     // no string payload
    ret->child_count = n;
    ret->children    = malloc(sizeof *ret->children * n);
    for (int i = 0; i < n; i++)
        ret->children[i] = va_arg(args, struct ASTNode *);
    
    va_end(args);
    return ret;
}

struct ASTNode *create_leaf_node(ASTNodeType type, const char *val) {
    struct ASTNode *ret = malloc(sizeof *ret);
    ret->type        = type;
    ret->val         = 0;                
    ret->value       = val ? strdup(val) : NULL;
    ret->child_count = 0;               
    ret->children    = NULL;            
    return ret;
}

/*void add_child_node(struct ASTNode *parent, struct ASTNode *child) {
    if (parent->child_count >= parent->child_capacity) {
        parent->child_capacity <<= 1;
        parent->children = (struct ASTNode **)realloc(parent->children, parent->child_capacity * sizeof(struct ASTNode *));
    }
    parent->children[parent->child_count++] = child;
}*/



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
const char *ast_node_type_to_string(ASTNodeType type) {
    switch (type) {
        case AST_FUNCTION_DEF:            return "FunctionDef";
        case AST_PARAM_LIST:              return "ParamList";
        case AST_PARAM:                   return "Param";
        case AST_DECLARATION:             return "Declaration";
        case AST_INIT_DECLARATOR:         return "InitDeclarator";
        case AST_ASSIGNMENT:              return "Assignment";
        case AST_PREPROCESSOR:            return "Preprocessor";
        case AST_PLUS:                    return "Plus";
        case AST_MINUS:                   return "Minus";
        case AST_MUL:                     return "Multiply";
        case AST_DIV:                     return "Divide";
        case AST_MOD:                     return "Modulo";
        case AST_LT:                      return "LessThan";
        case AST_GT:                      return "GreaterThan";
        case AST_LE:                      return "LessEqual";
        case AST_GE:                      return "GreaterEqual";
        case AST_FUNCTION_CALL:           return "FunctionCall";
        case AST_IDENTIFIER:              return "Identifier";
        case AST_LITERAL:                 return "Literal";
        case AST_NUMBER:                  return "Number";
        case AST_RETURN:                  return "Return";
        case AST_IF:                      return "If";
        case AST_ITERATION_STMT:          return "IterationStmt";
        case AST_COMPOUND_STMT:           return "CompoundStmt";
        case AST_ARRAY_ACCESS:            return "ArrayAccess";
        case AST_STRUCT_DECL:             return "StructDecl";
        case AST_TYPEDEF:                 return "Typedef";
        case AST_INIT_LIST:               return "InitList";
        case AST_POINTER_DEREF:           return "PointerDeref";
        case AST_ADDRESS_OF:              return "AddressOf";
        case AST_BREAK:                   return "Break";
        case AST_CONTINUE:                return "Continue";
        case AST_SIZEOF:                  return "Sizeof";
        case AST_POINTER:                 return "Pointer";
        case AST_TYPE_QUALIFIER:          return "TypeQualifier";
        case AST_TYPE_SPECIFIER:          return "TypeSpecifier";
        case AST_FUNCTION_SPECIFIER:      return "FunctionSpecifier";
        case AST_TRANS:                   return "TranslationUnit";
        case AST_LIST:                    return "List";
        case AST_BLOCK:                   return "Block";
        case AST_ALIGNMENT_SPECIFIER:     return "AlignmentSpecifier";
        case AST_STORAGE_CLASS_SPECIFIER: return "StorageClassSpecifier";
        case AST_DECLARATOR:              return "Declarator";
        case AST_ENUM:                    return "EnumSpecifier";
        case AST_TYPE_NAME:               return "TypeName";
        default:                         return "Unknown";
    }
}

void print_ast(struct ASTNode *node, int depth) {
    if (!node) return;
    for(int i = 0; i < depth; i++) putchar(' ');
    printf("%s", ast_node_type_to_string(node->type));
    if (node->value) {
        printf("(%s)", node->value);
    }
    putchar('\n');
    for(int i = 0; i < node->child_count; i++) {
        print_ast(node->children[i], depth + 2);
    }
}
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
    /*for (i = 0; i < 20 && (tok = yylex()); ++i)
        printf("tok[%d] = %d\n", i, tok);*/
    rewind(yyin);
    if (yyparse() == 0) {
        print_ast(root, 0);
        return 0;
    } else {
        fprintf(stderr, "Parse error\n");
        return 1;
    }
}