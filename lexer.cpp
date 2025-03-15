#include "file_io.hpp"
#include "token.hpp"
#include "fsm.hpp"
#include <vector>

int main(int argc, char **argv) {
    FILE *file = NULL;
    if ((file = open_file(argv[1])) != NULL) {
        FSM mach;
        std::vector<Token> tokens = mach.fsm(file);
        for (const auto& token : tokens) {
            token.describe();
        }

        close_file(argv[1], file);
    }
    return 0;
}