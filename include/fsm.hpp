#ifndef FSM_HPP 
#define FSM_HPP

#include "all.hpp"
#include "token.hpp"

class FSM {
public:
    vector<Token> fsm(FILE *file) {
        if (fgetc(file) == EOF) {
            throw Error("[FSM] The provided file is EMPTY! ");
            return {};
        }
    
        tState currState = tState::NewToken;
        tState nextState = tState::NewToken;
        string TokenUptillNow = "";
        Token currToken;
        currToken.set("", TOKEN_TYPE::UNKNOWN);

        char c;
        while ((c=fgetc(file)) != EOF) {
            switch (currState) {
                case tState::NewToken: {
                    TokenUptillNow = "";
                    currToken.set("", TOKEN_TYPE::UNKNOWN);

                    // for header files
                    if (c == '#') {
                        TokenUptillNow = "#";
                        nextState = tState::AcceptToken;
                        currToken.set(TokenUptillNow, TOKEN_TYPE::PREPROCESSOR);
                    }

                    else if (is_Symbol_Start(c)) {
                        TokenUptillNow = c;
                        nextState = tState::Symbol;
                        currToken.set(string(1, c), TOKEN_TYPE::SYMBOL);
                        /*while (is_Symbol(c) && c != EOF) {
                            TokenUptillNow += c;
                            c = fgetc(file);

                        }*/
                    }
                    
                    // handle comments
                    else if (c == '/') {
                        nextState = tState::Comment;
                    }

                    // else if (c == '')

                    // ignore spaces, newlines, tabs
                    else if (c == ' ' || c == '\t' || c == '\n') {
                        nextState = tState::NewToken;
                    }

                } break;

                /*case tState::Preprocessor: {
                    nextState = tState::AcceptToken;
                    TokenUptillNow = "";
                } break;*/

                case tState::Symbol: {
                    if (is_Symbol(c)) {
                        TokenUptillNow += c;
                        nextState = tState::Symbol;
                    }
                    else {
                        fseek(file, -1, SEEK_CURR); 
                        nextState = tState::AcceptToken;
                    }
                } break;

                case tState::AcceptToken: {
                    nextState = tState::NewToken;
                    fseek(file, -1, SEEK_CURR); // move back the pointer to run analysis 
                                                // for the current character as it failed 
                                                // the original condition.
                    return currToken;
                } break;

            }
            currState = nextState;
        }

        return currToken;
    }

};

#endif // FSM_HPP