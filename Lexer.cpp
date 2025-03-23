#include "Moore.hpp"
#include <chrono>
#include <fstream>

std::pair<std::string, std::pair<std::string, std::string>> get_num(std::ifstream &fin) {
    std::string num = "";
    std::string type = "DECIMAL";
    char ch = fin.get();
    num += ch;
    ch = fin.peek();
    
    if (ch == 'x' || ch == 'X') {
        type = "HEXADECIMAL_LITREAL";
        num += ch;
        fin.get();
    } else if (ch == 'b' || ch == 'B') {
        type = "BINARY_LITREAL";
        num += ch;
        fin.get();
    }
    
    while (true) {
        ch = fin.peek();
        if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'E') || (ch >= 'a' && ch <= 'e')) {
            num += ch;
            fin.get();
        } else {
            break;
        }
    }
    return {num, {"NUMBER", type}};
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    Trie* Moore = new Trie() ;
    
    std::ifstream fin("Input.cpp");
    std::ofstream fout("Output.txt");
    char ch;
    while (true) {
        ch = fin.peek();
        if (ch == EOF) break;
        if (ch == ' ' || isEscapeSequence(ch)) { 
            fin.get(); 
            continue; 
        }
        
        std::pair<std::string, std::pair<std::string, std::string>> tokenPair;
        
        if (ch >= '0' && ch <= '9')
            tokenPair = get_num(fin);
        else
            tokenPair = Moore->search(fin);
                
        std::string tokenStr = tokenPair.first;
        std::string output = tokenPair.second.first;
        std::string type = tokenPair.second.second;
        
        // std::cout << tokenStr << " " << output << " " << type << std::endl;
        
        fout << output << " ";
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    std::cout << "Execution Time: " << elapsed.count() << " seconds" << std::endl;
    
    return 0;
} 