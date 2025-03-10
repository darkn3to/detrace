#include "file_io.hpp"

bool open_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        cout << filename << " could not be opened.\n" << endl;
        return false;
    }
    else {
        cout << filename << " opened successfully.\n" << endl;
        fclose(file);
    }
    return true;
}

void close_file(const char *filename) {
    
}