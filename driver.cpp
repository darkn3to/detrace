#include "file_io.hpp"
#include "token.hpp"
#include "fsm.hpp"
#include "winnow.hpp"
#include "similarity.hpp"
#include "ast.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <stdbool.h>
#include <thread>
#include <chrono>
#include <filesystem>

namespace fs = filesystem;

extern ASTNode *root;

void process_file(const char *filename, bool originalFile) {
    FILE *file = open_file(filename); 
    if (file != NULL) {

        string fname_str(filename);
        FILE *logTokens = initializeOutputFile((fname_str + "_output.txt").c_str(), "tokens");
        //FILE *logFingerprints = initializeOutputFile((fname_str + "_fingerprints.txt").c_str(), "fingerprints");

        FSM mach;
        vector<Token> tokens = mach.fsm(file);
        for (const auto& token : tokens) {
            token.describe(logTokens, mach.iToken);
        }
        
        fseek(logTokens, 0, SEEK_SET);
        
        
        /*if (originalFile) {
            auto result = winnow(5, 3, logFingerprints, mach.iToken, mach.iToken.size(), true);
            if (result)
            orig_fingerprints = *result;
        }
        else 
            winnow(5, 3, logFingerprints, mach.iToken, mach.iToken.size(), false);
        */
        mach.iToken.clear();
        close_file(filename, file);
        fclose(logTokens);
        //fclose(logFingerprints);
    } 
    else {
        cerr << "Failed to open file: " << filename << endl;
    }
}

void ast_gen(const string path, const char *filename, const string orig, const char *benchmark) {
    //cout << "Generating AST for file: " << filename << endl;
    //string command = "./parser " + path + string(filename) + "_output.txt";
    string command = "./parser " + string(filename) + " " + orig + " " + string(benchmark);
    FILE *fp = popen(command.c_str(), "r");
    if (fp == NULL) {
        cerr << "Failed to run command: " << command << endl;
        return;
    }
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        cout << buffer;
    }
    pclose(fp);
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

    auto start = chrono::high_resolution_clock::now();

    // Approach 1 : Winnowing the Benchmark file
    /* -------------------------------------------------------------------------------------------------------------------------
    
    process_file(argv[1], true); // Process the original file separately
    for (const unsigned int &fingerprint : orig_fingerprints) {
        ++orig_fingerprintMap[fingerprint];
    }

     ------------------------------------------------------------------------------------------------------------------------- */

    string command = "bison -d ast.y";
    int gp = system(command.c_str());
    if (gp == -1) {
        cerr << "Failed to run command: " << command << endl;
        return 1;
    }
    command = "g++ ast.tab.c ast.cpp -o parser";
    gp = system(command.c_str());
    if (gp == -1) {
        cerr << "Failed to run command: " << command << endl;
        return 1;
    }
    process_file(argv[1], true); 
    ast_gen("analysis/tokens/", argv[1], "y", argv[1]);
    //read_orig_astMap_from_file();
    fs::create_directories("analysis/trees");   
    
    if (string(argv[argc - 1]) == "--m" || string(argv[argc - 1]) == "--M") {
        const unsigned int lprocs = max(1u, thread::hardware_concurrency());
        vector<std::thread> threads;
    
        for (int i = 2; i < argc - 1; ++i) {
            threads.emplace_back([filename = argv[i], originalFile = argv[1]]() {
                //  Continuation of Approach 1 : Winnowing the suspect files
                /*  -------------------------------------------------------------------------------------------------------------------------
                process_file(filename, false);
                    ---------------------------------------------------------------------------------------------------------------------- */
                string fpath = "analysis/fingerprints/" + string(filename) + "_fingerprints.txt";
                /* -------------------------------------------------------------------------------------------------------------------------
                jaccard_similarity(fpath, filename, originalFile);
                    ---------------------------------------------------------------------------------------------------------------------- */
                process_file(filename, false);
                ast_gen("analysis/tokens/", filename, "n", originalFile);
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
            // Continuation of Approach 1 : Winnowing the suspects; Non multithreaded
            /* -------------------------------------------------------------------------------------------------------------------------
            process_file(argv[i], false); 
               ----------------------------------------------------------------------------------------------------------------------- */
            string fpath = "analysis/fingerprints/" + string(argv[i]) + "_fingerprints.txt";
            /* -------------------------------------------------------------------------------------------------------------------------
            jaccard_similarity(fpath, argv[i], argv[1]);
                ----------------------------------------------------------------------------------------------------------------------- */
            process_file(argv[i], false);
            ast_gen("analysis/tokens/", argv[i], "n", argv[1]);
        }

    }
    else { 
        cerr << "Invalid option: " << argv[argc - 1] << endl;
        cerr << "Usage: " << argv[0] << " <original_file> <input_file> --m" << endl;
        return 1;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << duration.count() << " seconds" << endl;

    return 0;
}