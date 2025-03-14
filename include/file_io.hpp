#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include "all.hpp"
#include "log.hpp"

using namespace std;

static string msg = "";

FILE *open_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    loginit();
    msg = filename;
    if (!file) {
        msg += " could not be opened.";
        cout << msg << endl;
        log(msg);
        return NULL;
    }
    else {
        msg += " opened successfully.";
        cout << msg << endl;
        log(msg);
    }
    return file;
}

void close_file(const char *filename, FILE *file) {
    fclose(file);
    msg = filename; 
    msg += " closed successfully.";
    log(msg);
    cout << msg << endl;
}

#endif // FILE_IO_HPP