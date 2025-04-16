#include "Moore.hpp"
#include <chrono>
int main() {
    auto start = std::chrono::high_resolution_clock::now();    
    std::ifstream fin("test1.c");
    std::ofstream fout("Output1.txt");
    char ch ;
    std::string lexeme ;
    std::string token ;
    std::string type ;
        
    while (true) {
        ch = fin.peek(); 
        if (ch == EOF) break;
		else if(ch == ' ' || isEscapeSequence(ch)) { 
            fin.get(); 
            continue; 
        }
			
        std::pair<std::string, std::pair<std::string, std::string>> tokenPair;    
        tokenPair = search(fin);            
        lexeme = tokenPair.first;
        token = tokenPair.second.first;
        type = tokenPair.second.second;
		if(lexeme.empty()) continue ; 
        fout << token << " ";
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Execution Time: " << elapsed.count() << " seconds" << std::endl;
    return 0;
} 