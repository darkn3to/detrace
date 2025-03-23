#ifndef MOORE_HPP
#define MOORE_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <array>

inline bool isEscapeSequence(char ch) {
	int val = ch ;
    return ((val >= 7 && val <= 13) || val == 0 || val == 34 || val == 39 || val == 63 || val == 92) ? true : false ;
}
constexpr size_t CHILD_ARRAY_SIZE = 128;

class State {
private:
    char data;
    std::string type;
    std::string output;

public:
    std::array<State*, CHILD_ARRAY_SIZE> childs{};

    explicit State(char data) : data(data), type(""), output("") {
        childs.fill(nullptr);
    }

    void setOutputType(const std::string& output, const std::string& type) {
        this->output = output;
        this->type = type;
    }

    inline const std::string& getType() const {
        return type;
    }

    inline const std::string& getOutput() const {
        return output;
    }

    inline std::pair<std::string, std::string> getOutputType() const {
        return {output, type};
    }
};

class Trie {
public:

    State* root;
    Trie() {
        root = new State('\0');
        insertTokensInMoore();
    }

	inline void insertTokensInMoore() {
	    std :: ifstream fin("tokens_map.txt");
			
	    std :: string type, data, output;
	    while (fin >> data) {
	        if (data[0] == '\\') {
	            fin >> type;
	            continue;
	        }
	        fin >> output;
	        insertTokens(data, output, type);
	    }
	}

    inline void insertTokens(const std::string& word, const std::string& output, const std::string& type) {
        State* curr = root;
        for (unsigned char ch : word) {
            if (!curr->childs[ch]) {
                curr->childs[ch] = new State(ch);
            }
            curr = curr->childs[ch];
        }
        curr->setOutputType(output, type);
    }

    inline std::pair<std::string, std::pair<std::string, std::string>> search(std::ifstream& fin) {
        State* curr = root;
        std::string str;
        int ch = fin.peek();

        while (ch != EOF && curr->childs[ch]) {
            str.push_back(ch);
            curr = curr->childs[ch];
            fin.get();
            ch = fin.peek();
        }

        if (curr->getType().empty() || curr->getType() == "KEYWORD") {
            if (ch == ' ' || ch == EOF || isEscapeSequence(ch)) {
                return {str, curr->getType() == "KEYWORD" ? curr->getOutputType() : std::make_pair("IDENTIFIER", "IDENTIFIER")};
            }

            while ((ch = fin.peek()) != EOF) {
	 			if(((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch =='_'))){
                    str.push_back(ch);
                    fin.get();
                } else  break;
            }
            return {str, {"IDENTIFIER", "IDENTIFIER"}};
        }
        return {str, curr->getOutputType()};
    }
};
#endif