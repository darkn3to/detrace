#include "include/ast.hpp"
#include <iostream>
#include <map>
#include <set>
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

inline double calculate_bucket_score(const vector<unsigned int>& hashes, int size) {
    return size * hashes.size();
}

void AST::calculate_similarity() {
    double weighted_matching_score = 0.0;
    double total_score = 0.0;

    int sus_total_subtrees = 0;

    // Determine which map to use for total score calculation
    const auto& smaller_map = (orig_astMap.size() <= aux_astMap.size()) ? orig_astMap : aux_astMap;

    // Calculate total score (denominator) using the smaller map
    for (const auto &[size, hashes] : smaller_map) {
        total_score += calculate_bucket_score(hashes, size);
    }

    // Calculate weighted matching score (numerator)
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

            // Add weighted score for the matching subtrees
            weighted_matching_score += calculate_bucket_score(intersection, size);
        }
    }

    ifstream in("analysis/trees/count.txt");
    in >> orig_total_subtrees;
    cout << "Total original subtrees: " << orig_total_subtrees << "\n";
    cout << "Total suspect subtrees: " << sus_total_subtrees << "\n";

    cout << "Weighted Matching Score: " << weighted_matching_score << "\n";
    cout << "Total Score: " << total_score << "\n";

    double similarity = (total_score > 0) 
                        ? (weighted_matching_score / total_score) * 100.0 
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
        return {leaf_hash, 1};
    }

    unsigned int chs;
    unsigned int n;

    std::multimap<unsigned int, ASTNode*> func_def_buffer;
    std::vector<unsigned int> func_def_idx;

    for (int i = 0; i < node->child_count; ++i) {
        ast_chs(node->children[i], orig);

        if (node->children[i]->type == AST_FUNCTION_DEF) {
            func_def_buffer.emplace(node->children[i]->subtree_n_count, node->children[i]);
            func_def_idx.push_back(i);
        }
    }

    if (func_def_buffer.size() > 1) {
        auto it = func_def_buffer.begin();
        for (unsigned idx : func_def_idx) {
            node->children[idx] = it->second;
            ++it;
        }
    }

    func_def_buffer.clear();
    func_def_idx.clear();

    chs = static_cast<unsigned int>(node->type);
    n   = 1;
    for (int i = 0; i < node->child_count; ++i) {
        ASTNode* c = node->children[i];
        chs = chs * 31 + c->chs;
        n  += c->subtree_n_count;
    }

    node->chs             = chs;
    node->subtree_n_count = n;

    if (n >= 4) {
        if (orig == 'y') {
            auto& hashes = orig_astMap[n];
            hashes.insert(std::lower_bound(hashes.begin(), hashes.end(), chs), chs);
        } else {
            auto& hashes = aux_astMap[n];
            hashes.insert(std::lower_bound(hashes.begin(), hashes.end(), chs), chs);
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
