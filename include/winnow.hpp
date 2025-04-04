#ifndef WINNOW_HPP
#define WINNOW_HPP

#include "token.hpp"
#include <climits>
#include <cstring>

void winnow(const int window, const int k, FILE *file) {
    unordered_map<int, string> htable;
    htable.reserve(window);

    int iToken_size = iToken.size();  //iToken is in token.hpp

    unsigned short* circular_buffer = new unsigned short[k];

    int j = 0;
    for (int i = 0; i < k && j < iToken_size; i++, j++) {
        circular_buffer[i] = iToken[j];
    }
    
    /*cout << "Initial window: ";
    for (int i = 0; i < k; i++) {
        cout << circular_buffer[i] << " ";
    }
    cout << endl;
    */

    unsigned short rp = 0;
    
    while (j < iToken_size) {
        circular_buffer[rp] = iToken[j++];
        rp = (rp + 1) % k;
        /*cout << "Window: ";
        for (int i = 0; i < k; i++) {
            int idx = (rp + i) % k;
            cout << circular_buffer[idx] << " ";
        }
        cout << endl;
        */
    }

}

#endif // WINNOW_HPP