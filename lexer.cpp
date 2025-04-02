#include "file_io.hpp"
#include "token.hpp"
#include "fsm.hpp"
#include "winnow.hpp"
#include <vector>
#include <chrono>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file1> <input_file2> ... <input_fileN>" << endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        FILE *file = open_file(argv[i]);
        //auto start = chrono::high_resolution_clock::now();
        if (file != NULL) {
            
            string output_filename = string(argv[i]) + "_output.txt";
            FILE *out = fopen(output_filename.c_str(), "w");
            FSM mach;
            vector<Token> tokens = mach.fsm(file);
            for (const auto& token : tokens) {
                token.describe(out);
            }
            buffer += "\0";

            fseek(out, 0, SEEK_SET);
            //winnow(5, 3, out);

            close_file(argv[i], file);
            fclose(out);
        } else {
            cerr << "Failed to open file: " << argv[i] << endl;
        }

        /*auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        cout << duration.count() << " seconds" << endl;*/
    }


    return 0;
}