#include "include/ast.hpp"
#include <iostream>
#include <algorithm>
#include <utility>


ASTNode* root;
int orig_total_subtrees = 0;
unordered_map<unsigned int, vector<unsigned int>> orig_astMap;


void AST::count_subtrees() {
    orig_total_subtrees = 0;
    for (const auto &[size, hashes] : orig_astMap) {
        orig_total_subtrees += hashes.size();
    }
    ofstream out("analysis/trees/count.txt");
    out << orig_total_subtrees;
}

void AST::calculate_similarity() {
    int sus_total_subtrees = 0;
    int matching_subtrees = 0;
    
    for (const auto &[size, hashes] : aux_astMap) {
        sus_total_subtrees += hashes.size();

        if (orig_astMap.find(size) != orig_astMap.end()) {
            const auto &orig_hashes = orig_astMap.at(size);
            const auto &sus_hashes = hashes;

            vector<unsigned int> intersection;
            set_intersection(
                orig_hashes.begin(), orig_hashes.end(),
                sus_hashes.begin(), sus_hashes.end(),
                back_inserter(intersection)
            );

            matching_subtrees += intersection.size();
        }
    }

    ifstream in("analysis/trees/count.txt");
    in >> orig_total_subtrees;
    cout << "Total original subtrees: " << orig_total_subtrees << "\n";
    cout << "Total suspect subtrees: " << sus_total_subtrees << "\n";
    cout << "Matching subtrees: " << matching_subtrees << "\n";

    double similarity = (sus_total_subtrees > 0) 
                        ? (static_cast<double>(matching_subtrees) / min(orig_total_subtrees, sus_total_subtrees)) * 100.0 
                        : 0.0;

    cout << "Similarity: " << similarity << "%\n";
    cout << "--------------------------" << endl;
    cout << endl;
}

pair<unsigned int, unsigned int> AST::ast_chs(ASTNode *node, char orig) {
    if (!node) return {0, 0};

    // Leaf
    if (node->child_count == 0) {
        unsigned int leaf_hash = static_cast<unsigned int>(node->type);
        node->chs              = leaf_hash;
        node->subtree_n_count  = 1;

        /*if (orig == 'y') {
            auto &hashes = orig_astMap[1];
            hashes.insert(lower_bound(hashes.begin(), hashes.end(), leaf_hash), leaf_hash);
            //orig_total_subtrees++;
        } 
        else {
            auto &hashes = aux_astMap[1];
            hashes.insert(lower_bound(hashes.begin(), hashes.end(), leaf_hash), leaf_hash);
        }*/

        return {leaf_hash, 1};
    }

    unsigned int chs = static_cast<unsigned int>(node->type);
    unsigned int n   = 1;

    for (int i = 0; i < node->child_count; ++i) {
        auto [child_hash, child_count] = ast_chs(node->children[i], orig);
        chs  = chs * 31 + child_hash;
        n   += child_count;
    }

    node->chs             = chs;
    node->subtree_n_count = n;
    if (n >= 3) {
    if (orig == 'y') {
        auto &hashes = orig_astMap[n];
        hashes.insert(lower_bound(hashes.begin(), hashes.end(), chs), chs);
    } 
    else {
        auto &hashes = aux_astMap[n];
        hashes.insert(lower_bound(hashes.begin(), hashes.end(), chs), chs);
    }
}

    return {chs, n};
}

/*void group(struct ASTNode *node) {
    if (!node) return;
    
    orig_astMap[node->subtree_n_count].push_back(node->chs);

    for (int i = 0; i < node->child_count; i++) {
        group(node->children[i]);
    }
}*/