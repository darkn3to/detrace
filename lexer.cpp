#include "file_io.hpp"
#include "token.hpp"
#include "fsm.hpp"
#include <vector>
#include <chrono>

int main(int argc, char **argv) {
    FILE *file = NULL;
    if ((file = open_file(argv[1])) != NULL) {
        auto start = chrono::high_resolution_clock::now();
        FILE *out = fopen("out.txt", "w");
        FSM mach;
        vector<Token> tokens = mach.fsm(file);
        for (const auto& token : tokens) {
            token.describe(out);
        }

        close_file(argv[1], file);

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        cout << "Time taken: " << duration.count() << " seconds" << endl;
    }
    return 0;
}