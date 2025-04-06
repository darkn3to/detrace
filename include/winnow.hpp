#ifndef WINNOW_HPP
#define WINNOW_HPP

#define MOD 1000000007

#include "token.hpp"
#include <climits>
#include <cstring>
#include <vector>
#include <optional>

using namespace std;
using std::optional;

unsigned short* circular_buffer;

unsigned int rhash(const int k, const int rp) {
    unsigned int h = 0;
    int idx;
    for (int i = 0; i < k; i++) {
        idx = (rp + i) % k;
        h = (h * 31 + circular_buffer[idx]) % MOD;
    }
    return h;
}

optional<vector<unsigned int>> winnow(const int window, const int k, FILE *file, vector<unsigned short> &iToken, int iToken_size, bool ret) {
    
    vector<unsigned short> htable;
    vector<unsigned int> fingerprints;

    circular_buffer = new unsigned short[k];

    int j = 0, r = 0, min = 0;
    for (int i = 0; i < k && j < iToken_size; i++, j++) {
        circular_buffer[i] = iToken[j];
    }

    htable.push_back(rhash(k, 0));
    r = (r + 1) % window;
    

    unsigned short rp = 0;
    
    while (j < iToken_size) {
        circular_buffer[rp] = iToken[j++];
        rp = (rp + 1) % k;
        if (htable.size() < static_cast<unsigned short>(window)) {
            htable.push_back(rhash(k, rp));
        }
        else {
            htable[r] = rhash(k, rp);
        }
        if (min == r) {
            for (int i = (r - 1 + window) % window; i != r; i = (i - 1 + window) % window) {
                if (htable[i] < htable[min]) {
                    min = i;
                }
            }
            fingerprints.push_back(htable[min]);
        }
        else {
            if (htable[r] < htable[min]) {
                min = r;
                fingerprints.push_back(htable[min]);
            }
        }
        
        /*for (auto it : htable) {
            cout << it << " ";
        }
        cout << endl;*/
        
        r = (r + 1) % window;
    }

    /*for (auto it : fingerprints) {
        cout << it << " ";
    } */


    
    htable.clear();   //delete me if anything goes wrong
    delete[] circular_buffer;

    for (const unsigned int& fingerprint : fingerprints) {
        fprintf(file, "%d ", fingerprint); 
    }

    if (ret) 
        return fingerprints;
    else 
        return nullopt;
}

#endif // WINNOW_HPP
