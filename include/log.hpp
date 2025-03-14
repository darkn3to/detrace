#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <string>
#include <exception>

using namespace std;

static FILE *logFile = NULL;

void loginit() {
    if (!logFile)
        logFile = fopen("logs.txt", "w");
    else 
        logFile = fopen("logs.txt", "a");
}

inline void logClose() {
    fclose(logFile);
}

inline void log(const string message) {
    fprintf(logFile, "%s\n", message.c_str());
}

class Error : public exception {
    private:
    string message;

    public:
        Error(const string msg) {
            message = msg;
        }
        
        inline const char* what() const noexcept override {
            return message.c_str();
        }
};

#endif // LOG_HPP