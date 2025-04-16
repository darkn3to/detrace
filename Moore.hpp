#ifndef MOORE_HPP
#define MOORE_HPP
#include <iostream>
#include <fstream>
#include <array>

inline bool isEscapeSequence(char ch) {
	int val = ch ;
    return ((val >= 7 && val <= 13) || val == 0 || val == 92) ? true : false ;
}

inline std::pair<std::string, std::pair<std::string,std::string>> string_literal(std::ifstream &fin) {
	std :: string str = "";  
	str.push_back('"') ; 
	fin.get() ; 
	char ch = fin.get() ; 
	while(true){
		str.push_back(ch) ;
		if(ch == '"') break ;	
		ch = fin.get() ; 
	}
	fin.get() ;
	return {str,{"STRING_LITERAL","STRING_LITERAL"}} ;
}

inline std::pair<std::string, std::pair<std::string,std::string>> numeric_literal(std::ifstream &fin) {
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
        if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'E') || (ch >= 'a' && ch <= 'e') || ch == '.') {
            num += ch;
            fin.get();
			if(ch == '.') type = "FLOAT_LITERAL" ;
        	else if(ch == 'E' || ch == 'e'){
        		ch = fin.peek() ;
        		if(ch == '+' || ch == '-') {
					num += ch;
					fin.get() ;
				}
			}
        } 
		else {
            break;
        }
    }
    return {num, {"NUMBER", type}};
}

constexpr size_t CHILD_ARRAY_SIZE = 128;

class State {
public :
    char data;
    std::string type;
    std::string output;
    std::array<State*, CHILD_ARRAY_SIZE> childs{};

    explicit State(char data) : data(data), type(""), output("") {
        childs.fill(nullptr);
    }

    inline void setOutputType(const std::string& output, const std::string& type) {
        this->output = output;
        this->type = type;
    }

    inline std::pair<std::string, std::string> getOutputType() const {
        return {output, type};
    }
};

static State* Moore;	

inline std::pair<std::string, std::pair<std::string, std::string>> search(std::ifstream& fin) {
    char ch = fin.peek() ;
    
    std::string str;
    if (ch >= '0' && ch <= '9') return numeric_literal(fin);
    else if(ch == '"') return string_literal(fin) ;
    else if(ch == char(39)){
    	ch = fin.get() ;
    	str.push_back(ch) ; 
    	ch = fin.peek() ; 
	while(ch != char(39)) str.push_back((ch = fin.get())) ;
	return {str,{"CHAR_LITERAL","CHAR_LITERAL"}} ;
    }
    State* curr = Moore;
    while (ch != EOF && curr->childs[ch]) {
        str.push_back(ch);
        curr = curr->childs[ch];
        fin.get();
        ch = fin.peek();
    }
    
    if (curr->type.empty() || curr->type == "KEYWORD") {
	    if (ch == ' ' || ch == EOF || isEscapeSequence(ch)) {
	    	return {str, curr->type == "KEYWORD" ? curr->getOutputType() : std::make_pair("ID", "ID")};
		}
	
	    while ((ch = fin.peek()) != EOF) {
		 	if(((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch =='_'))){
	            str.push_back(ch);
	        	fin.get();
	        } else break;
	    }
	    return {str, {"ID","ID"}};
	}	

	if(curr->type == "CT"){
		if(str[1] == '/') while(ch != EOF && ch != '\n') ch = fin.get() ;
		else { 
			while(true){ 
			ch = fin.get() ; 
			if(ch == '*'){
				ch = fin.get() ; 
				if(ch == '/') break ;	
				}
			}
		}
		return {"",{"",""}} ; 
	}	
    return {str, curr->getOutputType()};
}
    
inline void insertTokensInMoore() {
	std :: ifstream fin("CTokens_map.txt");		
	std :: string type, data, output;
	while (fin >> data) {
	    if (data[0] == '\\') {
	        fin >> type;
	        continue;
	    }
	    fin >> output;
	    
		State* curr = Moore;
	   	for (char ch : data) {
		    if (!curr->childs[ch]) {
		        curr->childs[ch] = new State(ch);
		    }
		    curr = curr->childs[ch];
		}
		curr->setOutputType(output, type);
	}
}
    
struct MooreInitializer {
    MooreInitializer() {
	    Moore = new State('\0');
	    insertTokensInMoore();
    }
};

static MooreInitializer  mooreInitializer ;
#endif
