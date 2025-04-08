#ifndef FSM_HPP 
#define FSM_HPP

#include "all.hpp"
#include "token.hpp"
#include <vector>
#include <cstdio>
#include <string>

class FSM {
public:
    vector<unsigned short> iToken;

    string buffer;
    
    vector<Token> fsm(FILE *file) {
        vector<Token> tokens;
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        if (size == 0) {
            throw Error("[FSM] The provided file is EMPTY!");
        }
        
        this->buffer.resize(size);
        if ((long)fread(&this->buffer[0], 1, size, file) != size) {
            throw Error("[FSM] Failed to read the entire file into the buffer!");
        }
        
        string tokenBuffer;
        tokenBuffer.reserve(64);
        
        tState currState = tState::NewToken;
        tState nextState = tState::NewToken;
        Token currToken;
        size_t pos = 0;
        
        while (pos < this->buffer.size()) {
            char c = this->buffer[pos];
            switch (currState) {
                case tState::NewToken: {
                    tokenBuffer.clear();
                    currToken.set(tokenBuffer, TOKEN_TYPE::UNKNOWN);
                    if (c == '#') {
                        tokenBuffer.push_back('#');
                        while (++pos < this->buffer.size() && this->buffer[pos] != '\n') {
                            tokenBuffer.push_back(buffer[pos]);
                        }
                        currToken.set(tokenBuffer, TOKEN_TYPE::PREPROCESSOR);
                        nextState = tState::AcceptToken;
                    }
                    else if (c == '\'') {
                        pos++;
                        currToken.set(std::string(1, this->buffer[pos]), TOKEN_TYPE::CHAR_LITERAL);
                        pos++;
                        nextState = tState::AcceptToken;
                    }
                    else if (c == '"') {
                        tokenBuffer.push_back(c); 
                        while (++pos < this->buffer.size()) {
                            c = this->buffer[pos];
                            tokenBuffer.push_back(c);
                            if (c == '\\') { 
                                if (++pos < this->buffer.size()) {
                                    tokenBuffer.push_back(buffer[pos]);
                                }
                            } else if (c == '"') { 
                                break;
                            }
                        }
                        currToken.set(tokenBuffer, TOKEN_TYPE::STRING_LITERAL);
                        nextState = tState::AcceptToken;
                    }
                    
                    else if (is_Identifier_Start(c)) {
                        tokenBuffer.push_back(c);
                        currToken.set(tokenBuffer, TOKEN_TYPE::IDENTIFIER);
                        nextState = tState::Identifier;
                    }
                    else if (c >= '0' && c <= '9') {
                        tokenBuffer.push_back(c);
                        nextState = tState::Number;
                    }
                    else if (operatorMap.find(string(1, c)) != operatorMap.end()) {
                        tokenBuffer.push_back(c);
                        if (pos + 1 < this->buffer.size() && operatorMap.find(tokenBuffer + string(1, this->buffer[pos + 1])) != operatorMap.end()) {
                            ++pos; 
                            tokenBuffer.push_back(buffer[pos]);
                            //currToken.set(operatorMap[tokenBuffer]);
                        }
                        currToken.set(tokenBuffer, operatorMap[tokenBuffer]);
                        nextState = tState::AcceptToken;
                    }
                    else if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
                        nextState = tState::NewToken;
                    }
                    else if (c == '(' || c == ')' || c == '[' || c == ']' ||
                             c == '{' || c == '}') {
                        tokenBuffer = std::string(1, c);
                        if (c == '(')
                            currToken.set(tokenBuffer, TOKEN_TYPE::LPAREN);
                        else if (c == ')')
                            currToken.set(tokenBuffer, TOKEN_TYPE::RPAREN);
                        else if (c == '{')
                            currToken.set(tokenBuffer, TOKEN_TYPE::LBRACE);
                        else if (c == '}')
                            currToken.set(tokenBuffer, TOKEN_TYPE::RBRACE);
                        else if (c == '[')
                            currToken.set(tokenBuffer, TOKEN_TYPE::LSQBRACKET);
                        else if (c == ']')
                            currToken.set(tokenBuffer, TOKEN_TYPE::RSQBRACKET);
                        nextState = tState::AcceptToken;
                    }
                    else if (c == ':' || c == ';') {
                        tokenBuffer = string(1, c);
                        currToken.set(tokenBuffer, (c == ':') ? TOKEN_TYPE::COLON : TOKEN_TYPE::SEMICOLON);
                        nextState = tState::AcceptToken;
                    }
                    else if (c == ',' || c == '.') {
                        tokenBuffer = string(1, c);
                        currToken.set(tokenBuffer, (c == ',') ? TOKEN_TYPE::COMMA : TOKEN_TYPE::DOT);
                        nextState = tState::AcceptToken;
                    }
                    else if (c == '/') {
                        tokenBuffer.push_back(c);
                        nextState = tState::Symbol;
                    }
                    else {
                        tokenBuffer = std::string(1, c);
                        currToken.set(tokenBuffer, TOKEN_TYPE::UNKNOWN);
                        nextState = tState::AcceptToken;
                    }
                } break;
                
                case tState::Identifier: {
                    if (is_Identifier(c)) {
                        tokenBuffer.push_back(c);
                        nextState = tState::Identifier;
                    }
                    else {
                        if (keywordMap.find(tokenBuffer) != keywordMap.end()) {
                            currToken.set(tokenBuffer, keywordMap[tokenBuffer]);
                        }
                        else {
                            currToken.set(tokenBuffer, TOKEN_TYPE::IDENTIFIER);
                        }

                        if (pos > 0) pos--;
                        nextState = tState::AcceptToken;
                    }
                } break;

                case tState::Number: {
                    if (c >= '0' && c <= '9') {
                        tokenBuffer.push_back(c);
                        nextState = tState::Number;
                    }
                    else if (c == '.') {
                        tokenBuffer.push_back(c);
                        nextState = tState::Number;
                    }
                    else if (c == 'e' || c == 'E') {
                        tokenBuffer.push_back(c);
                        nextState = tState::Number;
                    }
                    else {
                        if (pos > 0) pos--;
                        currToken.set(tokenBuffer, TOKEN_TYPE::NUMBER);
                        nextState = tState::AcceptToken;
                    }
                } break;
                
                case tState::Unknown: {
                    if (pos > 0) pos--;
                    nextState = tState::AcceptToken;
                } break;
                
                case tState::AcceptToken: {
                    tokens.push_back(currToken);
                    nextState = tState::NewToken;

                    if (pos > 0) pos--;
                } break;
                
                default: {
                    throw Error("[FSM] Unhandled state encountered!");
                } break;
            }
            pos++;
            currState = nextState;
        }
        
        if (!tokenBuffer.empty()) {
            currToken.set(tokenBuffer, currToken.type);
            tokens.push_back(currToken);
        }
        //added this
        this->buffer.clear();
        return tokens;
    }

};

#endif // FSM_HPP
