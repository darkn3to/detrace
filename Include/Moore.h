#include "TOKENS.h"
#include<fstream>
class TrieNode{
    private : 
        char data ; 
        string type ; 
        string output ; 
        
    public : 
        unordered_map<char,TrieNode*>childs ;
        
        TrieNode(char data){
           this->data = data ; 
           this->output = "" ;
  		   this->type = "" ;  
        } 
        
        void set_output_type(string output,string type){
            this->output = output ;
			this->type = type ;  
        } 
    
        string getType(){
			return this->type ;  
        } 
    
        string getOutput(){
            return this->output ;
        } 
    
        pair<string,string> get_output_type(){
            return {this->output,this->type} ; 
        } 
};

class Trie {
public:
    TrieNode *root ; 
    Trie() {
        root = new TrieNode('\0') ; 
    }
    
    void insertTokens(string word,string output,string type) {
        int n = word.length() ; 
        TrieNode *curr = root ; 

        for(int i = 0 ; i<n ; i++){
            if(curr->childs.find(word[i]) != curr->childs.end()){
                curr = curr->childs[word[i]] ;
            }else{
                TrieNode *child = new TrieNode(word[i]) ; 
                curr->childs[word[i]] = child ; 
                curr = child ;
            }
        }
        curr->set_output_type(output,type) ; 
    }
    
    pair<string,pair<string,string>> search(ifstream &fin) {
        TrieNode *curr = root ; 
        string str = "" ;
        char ch ; 
        while((ch = fin.peek()) != EOF) {  
            if(curr->childs.find(ch) != curr->childs.end()){
            	str+=ch ;
                curr = curr->childs[ch] ;
                fin.get() ; 
            }else break  ;
        } 
		
		
		if(curr->getType().empty() || curr->getType() == "KEYWORD"){ 
			if(ch == ' ' || (ch == EOF) || (is_escape_sequence(ch))){
		 	  if(curr->getType() =="KEYWORD") return {str,curr->get_output_type()} ;
			  else return {str,{"INDENTIFIER","INDENTIFIER"}} ;	
			}

			while(true){ 
				ch = fin.peek() ;
				if(ch == EOF) break ; 
	 			if(((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch =='_')) ){
				 	str += ch ; 
				 	fin.get() ;
				}
				break ; 
			}
			
		 return {str,{"INDENTIFIER","INDENTIFIER"}} ;
		}
		
		return {str,curr->get_output_type()} ;	
    }
    
};


void insert_tokens_in_moore(Trie *Moore){
	string type , data , output ;
	for(auto token : TOKENS){
		type = token.first ; 
		for(auto p : token.second){
			data = p.first ; 
			output = p.second ; 
			Moore->insertTokens(data,output,type) ;
		}
	}
}


Trie* create_Moore_Machine(){
	Trie* Moore = new Trie() ;
	define_tokens() ;
	insert_tokens_in_moore(Moore) ;
	return Moore ; 
}
