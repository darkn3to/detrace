#ifndef WINNOW_HPP
#define WINNOW_HPP

#define MOD 1000000007

#include "token.hpp"
#include <cstring>
#include <vector>

using namespace std;

unsigned short* circular_buffer;

unsigned long rhash(const int k, const int rp) {
    unsigned long h = 0;
    int idx;
    for (int i = 0; i < k; i++) {
        idx = (rp + i) % k;
        h = (h * 31 + circular_buffer[idx]) % MOD;
    }
    return h;
}

void winnow(const int window, const int k, FILE *file) {
    
    vector<unsigned short> htable;
    vector<unsigned long> fingerprints;

    int iToken_size = iToken.size();  // iToken is defined in token.hpp

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

    for (auto it : fingerprints) {
        cout << it << " ";
    } 
    
    delete[] circular_buffer;
}

#endif // WINNOW_HPP
