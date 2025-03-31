#ifndef WINNOW_HPP
#define WINNOW_HPP

#include "token.hpp"
#include <climits>
#include <cstring>
#include <deque>

void print(deque<string> &dq) {
    for (const auto& str : dq) {
        cout << str << " ";
    }
    cout << endl;
}

void winnow(const int window, const int k, FILE *file) {
    unordered_map<int, string> htable;
    htable.reserve(window);

    string temp;
    temp.reserve(64 * k);
    //unordered_map<string, string> prehash;
    //int rt = 0, minimum = 0;
    short i = 0;

    char *token = strtok(buffer.data(), " ");
    //int lp = 0, rp = k-1;
    deque<string> dq;

    while (i < k && token != nullptr) {
        dq.push_back(token);
        token = strtok(nullptr, " ");
        ++i;
    }

    //htable[rt] ;

    while (token != nullptr) {
        //cout << token << endl;
        //rt = (rt + 1) % window;
        print(dq);
        dq.pop_front();
        token = strtok(nullptr, " ");
        if (token != nullptr) { 
            dq.push_back(token);
        }
    }

}

#endif // WINNOW_HPP