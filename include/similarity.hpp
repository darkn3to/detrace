#ifndef SIMILARITY_HPP
#define SIMILARITY_HPP

#include <mutex>
std::mutex cout_mutex;

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"


// remove all printing, just use all the buffers that you used
#include "all.hpp"
#include <unordered_map>

int intersection(const string &filename, FILE *fi) {
    int res = 0;
    if (fi == NULL) {
        cerr << "Error: Could not open file: " << filename << endl;
        //return vector<unsigned int>();
    }

    unordered_map<unsigned int, unsigned short> fingerprintMap;
    unsigned int value;
    while (fread(&value, sizeof(unsigned int), 1, fi) == 1) {
        //cout << value << " ";
        ++fingerprintMap[value];
    }

    for (const auto &fm : orig_fingerprintMap) {
        if (fingerprintMap.find(fm.first) != fingerprintMap.end()) {
            res += min(fm.second, fingerprintMap[fm.first]);
        }
    }

    return res;
}

inline int _union(FILE *fi, const int iRes) {
    return orig_fingerprints.size() + (ftell(fi) / sizeof(unsigned int)) - iRes;
}

void jaccard_similarity(const string &filename, const string &inp_file, const string &benchmark) {
    FILE *fi = fopen(filename.c_str(), "rb");
    int iRes = intersection(filename, fi);
    int uRes = _union(fi, iRes);
    {
    std::lock_guard<std::mutex> lock(cout_mutex);
    cout << "--------------------------" << endl;
    cout << "Similarity Report for File: " << inp_file << endl;
    cout << "Comparing against: " << benchmark << endl;
    cout << "Intersection: " << iRes << endl;
    cout << "Union: " << uRes << endl;
    double sim = (static_cast<double>(iRes) / uRes) * 100;
    cout << "Similarity: " ;
    if (sim >= 75) 
        cout << RED << sim << "%" << RESET << endl;
    else if (sim >= 45) 
        cout << YELLOW << sim << "%" << RESET << endl;
    else 
        cout << GREEN << sim << "%" << RESET << endl;

    cout << "--------------------------" << endl;
    }
    fclose(fi);
}
    

#endif // SIMILARITY_HPP