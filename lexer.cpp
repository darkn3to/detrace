#include "file_io.hpp"
#include "token.hpp"
#include "fsm.hpp"
#include <vector>
#include <chrono>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file1> <input_file2> ... <input_fileN>" << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        FILE *file = open_file(argv[i]);
        auto start = std::chrono::high_resolution_clock::now();
        if (file != NULL) {
            
            std::string output_filename = std::string(argv[i]) + ".txt";
            FILE *out = fopen(output_filename.c_str(), "w");
            FSM mach;
            std::vector<Token> tokens = mach.fsm(file);
            for (const auto& token : tokens) {
                token.describe(out);
            }

            close_file(argv[i], file);
            fclose(out);
        } else {
            std::cerr << "Failed to open file: " << argv[i] << std::endl;
        }

        auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << duration.count() << " seconds" << std::endl;
    }


    return 0;
}