#include "Moore.h"
#include <chrono>

//unordered_map<string,vector<string>>Tokens ; // Tokens which are founded in the input file 
 
pair<string,pair<string,string>> get_num(ifstream &fin){

		string num = "" ;
		string type = "DECIMAL" ;
		char ch = fin.get() ;
		num += ch ; 
		ch = fin.peek() ;
		
		if(ch == 'x' || ch == 'X') { type = "HEXADECIMAL_LITREAL" ; num+=ch ; fin.get() ; }
		else if(ch == 'b' || ch == 'B') {type = "BINARY_LITREAL" ; num+=ch;  fin.get() ; }
		
		while(true){
			ch = fin.peek() ;
			if((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'E') || (ch >= 'a' && ch <= 'e')){
				num += ch ; 
				fin.get() ;
			}else break ; 
		}
		return {num,{"NUMBER",type}} ;
}

int main(){
	auto start = std::chrono::high_resolution_clock::now();

	Trie *Moore = create_Moore_Machine() ; 
	ifstream fin("Input.cpp") ;
	ofstream fout("Output.txt") ; 
	
	char ch ;
	int count = 0 ;
		
	while(true){
		ch = fin.peek() ;
		
		if(ch == EOF) break ; 
		if(ch == ' ' || is_escape_sequence(ch)) { fin.get() ; continue ; }
		
		pair<string,pair<string,string>> pp  ;

		if(ch >= '0' && ch <= '9') pp = get_num(fin) ;
		else pp = Moore->search(fin) ;
				
		string str = pp.first ;  
		string output = (pp.second).first ; 
		string type = (pp.second).second ; 
		
//		cout << str << " " << output << " " << type << endl  ; 
		fout << output << " " ; 
	}
	
	auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Execution Time: " << elapsed.count() << " seconds" << std::endl;

	
}