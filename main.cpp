#include "file_io.hpp"

int main(int argc, char **argv) {

    if (open_file(argv[1])) {


        close_file(argv[1]);
    }
    return 0;
}
