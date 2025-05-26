#ifndef AST_H
#define AST_H

#include <vector>
#include <unordered_map>
#include <fstream>
using namespace std;

#define makelist(x) create_ast_node(AST_LIST, 1, x)
#define makelist_leaf(x) create_leaf_node(AST_LIST, x)
#define append_list(x1, x2) ((x1 && x2) ? create_ast_node(AST_LIST, 2, x1, x2) : (x1) ? x1 : (struct ASTNode *)NULL)

typedef enum {
    AST_FUNCTION_DEF = 1,
    AST_PARAM_LIST,
    AST_PARAM,
    AST_DECLARATION,
    AST_INIT_DECLARATOR,
    AST_ASSIGNMENT,
    AST_PREPROCESSOR,
    AST_PLUS, AST_MINUS, AST_MUL, AST_DIV, AST_MOD,
    AST_LT, AST_GT, AST_LE, AST_GE,
    AST_AND, AST_OR, AST_NOT,
    AST_BIT_AND, AST_BIT_OR, AST_BIT_XOR,
    AST_BIT_NOT, AST_BIT_LEFT_SHIFT, AST_BIT_RIGHT_SHIFT,
    AST_EQUAL, AST_NOT_EQUAL,
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
    AST_CONTINUE,
    AST_SHIFT_EXPR,
    AST_UNARY_OP,
    AST_INCREMENT,
    AST_DECREMENT
} ASTNodeType;

struct ASTNode {
    ASTNodeType type;
    int subtree_n_count, chs;
    char *value;
    int child_count;
    struct ASTNode **children;
};

extern std::unordered_map<unsigned int, std::vector<unsigned int>> orig_astMap;
extern std::unordered_map<unsigned int, std::vector<unsigned int>> aux_astMap;

// ast creation functions
struct ASTNode *create_ast_node(ASTNodeType type, int n, ...);
struct ASTNode *create_leaf_node(ASTNodeType type, const char *val);

// ast minimization functions
vector<struct ASTNode *> splice_func(struct ASTNode *list);
vector<struct ASTNode *> splice(struct ASTNode *list);
vector<struct ASTNode *> flatten_decl(struct ASTNode *node);
struct ASTNode *flatten_ast(struct ASTNode *node);

void print_ast(struct ASTNode *node, int depth);

extern int orig_total_subtrees;

// astcc algo functions
class AST {
public: 
    unordered_map<unsigned int, vector<unsigned int>> aux_astMap;
    pair<unsigned int, unsigned int> ast_chs(struct ASTNode *root, char orig);
    void printMaps(const char) const;
    void count_subtrees();
    void calculate_similarity();
};

#endif // AST_H
