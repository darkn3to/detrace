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

        vector<Token> vecOutputTokens;
    
        tState currState = tState::NewToken;
        tState nextState = tState::NewToken;
        string TokenUptillNow = "";
        Token currToken;

        char c;
        while ((c=fgetc(file)) != EOF) {
            switch (currState) {
                case tState::NewToken:
                    TokenUptillNow = "";
                    currToken.type = TOKEN_TYPE::UNKNOWN;

                    // for header files
                    if (c == '#') {
                        TokenUptillNow = "#";
                        currState = tState::Preprocessor;
                    }

                }

            currState = nextState;
        }

        return vecOutputTokens;
    }

};

#endif // FSM_HPP