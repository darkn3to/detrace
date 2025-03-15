#include "file_io.hpp"
#include "token.hpp"
#include "fsm.hpp"

int main(int argc, char **argv) {
    FILE *file = NULL;
    if ((file=open_file(argv[1])) != NULL) {
        FSM mach;
        while (fgetc(file) != EOF) {
            Token outToken = mach.fsm(file);
            cout << outToken.type << endl;
        }

        close_file(argv[1], file);
    }
    return 0;
}
