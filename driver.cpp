#include "file_io.hpp"
#include "token.hpp"
#include "fsm.hpp"
#include "winnow.hpp"
#include "similarity.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <stdbool.h>
#include <thread>
#include <chrono>
#include <filesystem>

namespace fs = filesystem;

void process_file(const char *filename, bool originalFile) {
    FILE *file = open_file(filename); 
    if (file != NULL) {

        string fname_str(filename);
        FILE *logTokens = initializeOutputFile((fname_str + "_output.txt").c_str(), "tokens");
        FILE *logFingerprints = initializeOutputFile((fname_str + "_fingerprints.txt").c_str(), "fingerprints");

        FSM mach;
        vector<Token> tokens = mach.fsm(file);
        for (const auto& token : tokens) {
            token.describe(logTokens, mach.iToken);
        }
        
        fseek(logTokens, 0, SEEK_SET);
        
        
        if (originalFile) {
            auto result = winnow(5, 3, logFingerprints, mach.iToken, mach.iToken.size(), true);
            if (result)
            orig_fingerprints = *result;
        }
        else 
            winnow(5, 3, logFingerprints, mach.iToken, mach.iToken.size(), false);

        mach.iToken.clear();
        close_file(filename, file);
        fclose(logTokens);
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

    auto start = chrono::high_resolution_clock::now();

    // Approach 1 : Winnowing the Benchmark file
    /* -------------------------------------------------------------------------------------------------------------------------
    
    process_file(argv[1], true); // Process the original file separately
    for (const unsigned int &fingerprint : orig_fingerprints) {
        ++orig_fingerprintMap[fingerprint];
    }

     ------------------------------------------------------------------------------------------------------------------------- */
    
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