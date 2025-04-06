#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <string>
#include <filesystem>
#include <exception>

using namespace std;

namespace fs = filesystem;

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

FILE* initializeOutputFile(const string& filename) {
    const string parentDir = "analysis";
    const string fingerprintsDir = parentDir + "/fingerprints";

    try {
        if (!fs::exists(parentDir)) {
            fs::create_directories(parentDir);
        }

        if (!fs::exists(fingerprintsDir)) {
            fs::create_directories(fingerprintsDir);
        }

        string fullPath = fingerprintsDir + "/" + filename;

        FILE* file = fopen(fullPath.c_str(), "w");
        if (file == NULL) {
            cerr << "Error: Could not create file: " << fullPath << endl;
            return NULL;
        }

        cout << "File initialized at: " << fullPath << endl;
        return file;
    } catch (const fs::filesystem_error& e) {
        cerr << "Error creating directories: " << e.what() << endl;
        return NULL;
    }
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