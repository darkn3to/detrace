#include "file_io.hpp"

FILE *open_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        cout << filename << " could not be opened.\n" << endl;
        return NULL;
    }
    else {
        cout << filename << " opened successfully.\n" << endl;
    }
    return file;
}

void close_file(const char *filename, FILE *file) {
    fclose(file);
    cout << filename << " closed successfully.\n" << endl;
}