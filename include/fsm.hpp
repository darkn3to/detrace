#ifndef FSM_HPP 
#define FSM_HPP

#include "all.hpp"
#include "token.hpp"
#include <vector>

class FSM {
public:
    std::vector<Token> fsm(FILE *file) {
        std::vector<Token> tokens;
        
        short initialPos = static_cast<short>(ftell(file));

        if (fgetc(file) == EOF) {
            throw Error("[FSM] The provided file is EMPTY! ");
            return tokens;
        }

        fseek(file, initialPos, SEEK_SET);
    
        tState currState = tState::NewToken;
        tState nextState = tState::NewToken;
        string TokenUptillNow = "";
        Token currToken;

        char c;
        while ((c = fgetc(file)) != EOF) {
            switch (currState) {
                case tState::NewToken: {
                    TokenUptillNow = "";
                    currToken.set(TokenUptillNow, TOKEN_TYPE::UNKNOWN);

                    // for header files
                    if (c == '#') {
                        TokenUptillNow = "#";
                        while ((c = fgetc(file)) != '\n' && c != EOF) {
                            TokenUptillNow += string(1, c);
                            //cout << "tok: " << TokenUptillNow << endl;
                        }
                        //cout << "Hash detected" << endl;
                        currToken.set(TokenUptillNow, TOKEN_TYPE::PREPROCESSOR);
                        nextState = tState::AcceptToken;
                    }

                    else if (is_Symbol_Start(c)) {
                        //cout << "Symbol start detected" << endl;
                        TokenUptillNow = c;

                        nextState = tState::Symbol;
                        currToken.set(string(1, c), TOKEN_TYPE::SYMBOL);
                    }
                    
                    /* handle comments
                    else if (c == '/') {
                        nextState = tState::Comment;
                    }*/

                    // ignore spaces, newlines, tabs, carriage returns
                    else if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
                        nextState = tState::NewToken;
                    }

                    else {
                        nextState = tState::Unknown;
                        TokenUptillNow = string(1, c);
                    }

                } break;

                case tState::Symbol: {
                    //cout << "Symbol detected:  " << c << endl;
                    if (is_Symbol(c)) {
                        TokenUptillNow += c;
                        //cout << TokenUptillNow << endl;
                        nextState = tState::Symbol;
                    }
                    else {
                        fseek(file, -1, SEEK_CUR); 
                        nextState = tState::AcceptToken;
                    }
                } break;

                case tState::Unknown: {
                    fseek(file, -1, SEEK_CUR);
                    nextState = tState::AcceptToken;
                } break;

                case tState::AcceptToken: {
                    //cout << "Accepting token: " << TokenUptillNow << endl;
                    nextState = tState::NewToken;
                    fseek(file, -1, SEEK_CUR);  // move back the pointer to run analysis 
                                                // for the current character as it failed 
                                                // the original condition.      

                    currToken.set(TokenUptillNow, currToken.type);
                    tokens.push_back(currToken);
                    //cout << "Token accepted: " << currToken.lexeme << endl;
                } break;

                default: {
                    throw Error("[FSM] Unhandled state encountered!");
                } break;
            }
            currState = nextState;
        }
        
        //if (currState == tState::Symbol || currState == tState::AcceptToken) {
        currToken.set(TokenUptillNow, currToken.type);
        tokens.push_back(currToken);

        return tokens;
    }

};

#endif // FSM_HPP