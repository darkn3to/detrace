#include "file_io.hpp"
#include "token.hpp"
int main(int argc, char **argv) {
    FILE *file = NULL;
    if ((file=open_file(argv[1])) != NULL) {


        close_file(argv[1], file);
    }
    return 0;
}
