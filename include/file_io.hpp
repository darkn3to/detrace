#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include "all.hpp"

using namespace std;
FILE *open_file(const char *filename);

void close_file(const char *filename, FILE *file);

#endif // FILE_IO_HPP