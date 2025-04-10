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

unsigned int rhash(const int k, const int rp, const vector<unsigned short>& circular_buffer) {
    unsigned int h = 0;
    int idx;
    for (int i = 0; i < k; i++) {
        idx = (rp + i) % k;
        h = (h * 31 + circular_buffer[idx]) % MOD;
    }
    return h;
}

optional<vector<unsigned int>> winnow(const int window, const int k, FILE * const file, vector<unsigned short> &iToken, int iToken_size, bool ret) {
    
    vector<unsigned short> htable;
    vector<unsigned int> fingerprints;
    vector<unsigned short> circular_buffer(k);

    int j = 0, r = 0, min = 0;
    for (int i = 0; i < k && j < iToken_size; i++, j++) {
        circular_buffer[i] = iToken[j];
    }

    htable.push_back(rhash(k, 0, circular_buffer));
    r = (r + 1) % window;
    

    unsigned short rp = 0;
    
    while (j < iToken_size) {
        circular_buffer[rp] = iToken[j++];
        rp = (rp + 1) % k;
        if (htable.size() < static_cast<unsigned short>(window)) {
            htable.push_back(rhash(k, rp, circular_buffer));
        }
        else {
            htable[r] = rhash(k, rp, circular_buffer);
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


    //delete[] circular_buffer;
    //circular_buffer.clear();
    //circular_buffer = nullptr; //delete me too

    for (const unsigned int& fingerprint : fingerprints) {
        fwrite(&fingerprint, sizeof(unsigned int), 1, file);
        //fprintf(file, "%d ", fingerprint); // for text output
    }
    
    if (ret) 
        return fingerprints;
    else 
        return nullopt;
}

#endif // WINNOW_HPP
