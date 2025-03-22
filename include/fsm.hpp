#ifndef FSM_HPP 
#define FSM_HPP

#include "all.hpp"
#include "token.hpp"
#include <vector>
#include <cstdio>
#include <string>

class FSM {
public:
    std::vector<Token> fsm(FILE *file) {
        std::vector<Token> tokens;
        
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (size == 0) {
            throw Error("[FSM] The provided file is EMPTY!");
            return tokens;
        }

        std::string buffer(size, '\0');
        fread(&buffer[0], 1, size, file);

        tState currState = tState::NewToken;
        tState nextState = tState::NewToken;
        std::string TokenUptillNow = "";
        Token currToken;
        
        size_t pos = 0;
        while (pos < buffer.size()) {
            char c = buffer[pos];
            switch (currState) {
                case tState::NewToken: {
                    TokenUptillNow = "";
                    currToken.set(TokenUptillNow, TOKEN_TYPE::UNKNOWN);
                    if (c == '#') {
                        TokenUptillNow = "#";
                        while (++pos < buffer.size() && buffer[pos] != '\n') {
                            TokenUptillNow += buffer[pos];
                        }
                        currToken.set(TokenUptillNow, TOKEN_TYPE::PREPROCESSOR);
                        nextState = tState::AcceptToken;
                    }
                    else if (c == '"') {
                        while (++pos < buffer.size() && buffer[pos] != '"') {
                            TokenUptillNow += buffer[pos];
                        }
                        currToken.set(TOKEN_TYPE::STRING_LITERAL);
                        nextState = tState::AcceptToken;
                    }
                    else if (is_Identifier_Start(c)) {
                        TokenUptillNow = c;
                        nextState = tState::Identifier;
                        currToken.set(std::string(1, c), TOKEN_TYPE::IDENTIFIER);
                    }
                    else if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
                        nextState = tState::NewToken;
                    }
                    else if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}') {
                        nextState = tState::AcceptToken;
                        TokenUptillNow = std::string(1, c);
                        if (c == '(') {
                            currToken.set(TOKEN_TYPE::LPAREN);
                        }
                        else if (c == ')') {
                            currToken.set(TOKEN_TYPE::RPAREN);
                        }
                        else if (c == '{') {
                            currToken.set(TOKEN_TYPE::LBRACE);
                        }
                        else if (c == '}') {
                            currToken.set(TOKEN_TYPE::RBRACE);
                        }
                        else if (c == '[') {
                            currToken.set(TOKEN_TYPE::LSQBRACKET);
                        }
                        else if (c == ']') {
                            currToken.set(TOKEN_TYPE::RSQBRACKET);
                        }
                    }
                    else if (c == ':' || c == ';') {
                        TokenUptillNow = std::string(1, c);
                        currToken.set(TokenUptillNow, (c == ':') ? TOKEN_TYPE::COLON : TOKEN_TYPE::SEMICOLON);
                        nextState = tState::AcceptToken;
                    }
                    else {
                        nextState = tState::Unknown;
                        TokenUptillNow = std::string(1, c);
                    }
                } break;

                case tState::Identifier: {
                    if (is_Identifier(c)) {
                        TokenUptillNow += c;
                        nextState = tState::Identifier;
                    }
                    else {
                        if (keywordMap.find(TokenUptillNow) != keywordMap.end()) {
                            currToken.set(TOKEN_TYPE::KEYWORD);
                        }
                        if (pos > 0) pos--;
                        nextState = tState::AcceptToken;
                    }
                } break;

                case tState::Unknown: {
                    if (pos > 0) pos--;
                    nextState = tState::AcceptToken;
                } break;

                case tState::AcceptToken: {
                    nextState = tState::NewToken;
                    if (pos > 0) pos--;
                    currToken.set(TokenUptillNow, currToken.type);
                    tokens.push_back(currToken);
                } break;

                default: {
                    throw Error("[FSM] Unhandled state encountered!");
                } break;
            }
            pos++;
            currState = nextState;
        }
        
        currToken.set(TokenUptillNow, currToken.type);
        tokens.push_back(currToken);

        return tokens;
    }
};

#endif // FSM_HPP
