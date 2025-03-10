#include <iostream>
using namespace std;

int main(int argc, char **argv) {

    if (open_file(argv[1])) {

    }
    else {
        cout << "File could not be opened" << endl;
    }
    return 0;
}
