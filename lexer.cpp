#include "file_io.hpp"
#include "token.hpp"
#include "fsm.hpp"
#include "winnow.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <stdbool.h>
#include <thread>
#include <filesystem>

namespace fs = filesystem;

vector<unsigned int> orig_fingerprints;

void process_file(const char *filename, bool originalFile) {
    FILE *file = open_file(filename); // Use filename directly
    if (file != NULL) {
        string output_filename = string(filename) + "_output.txt";
        
        FILE *out = fopen(output_filename.c_str(), "w");\
        FILE *logFingerprints = initializeOutputFile(std::string(filename) + "_fingerprints.txt");
        FSM mach;
        vector<Token> tokens = mach.fsm(file);
        for (const auto& token : tokens) {
            token.describe(out, mach.iToken);
        }
        buffer += "\0";
        
        fseek(out, 0, SEEK_SET);
        
        if (originalFile) {
            auto result = winnow(5, 3, logFingerprints, mach.iToken, mach.iToken.size(), true);
            if (result)
            orig_fingerprints = *result;
        }
        else 
            winnow(5, 3, logFingerprints, mach.iToken, mach.iToken.size(), false);

        mach.iToken.clear();
        close_file(filename, file);
        fclose(out);
        fclose(logFingerprints);
    } 
    else {
        cerr << "Failed to open file: " << filename << endl;
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        //cerr << "Usage: " << argv[0] << " <original_file> <input_file1> <input_file2> ... <input_fileN>" << endl;
        cerr << "Usage: " << argv[0] << " <original_file> <input_file>" << endl;
        return 1;
    }

    if (!fs::exists("analysis")) {
        fs::create_directories("analysis");
    }

    process_file(argv[1], true); // Process the original file separately
    buffer.clear();
    
    if (string(argv[argc - 1]) == "--m" || string(argv[argc - 1]) == "--M") {
        unsigned const int lprocs = thread::hardware_concurrency();
        vector<std::thread> threads;
    
        for (int i = 2; i < argc - 1; ++i) {
            threads.emplace_back([=]() {
                process_file(argv[i], false);
            });

            if (threads.size() >= lprocs) {
                for (auto& t : threads) {
                    t.join(); 
                }
                threads.clear(); 
            }
        }
    
        for (auto& t : threads) {
            t.join();
        }
    }
    else if (string(argv[argc - 1]) != "--m" && string(argv[argc - 1]) != "--M") {
        for (int i = 2; i < argc; ++i) {
            process_file(argv[i], false); 
        }
    }
    else {
        cerr << "Invalid option: " << argv[argc - 1] << endl;
        cerr << "Usage: " << argv[0] << " <original_file> <input_file> --m" << endl;
        return 1;
    }

    return 0;
}