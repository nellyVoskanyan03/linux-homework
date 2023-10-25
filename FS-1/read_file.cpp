#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Please provide the file path " << std::endl;
        return 1;
    }

    const char* path = argv[1];
    std::ifstream file(path);

    if (!file) {
        std::cerr << "No such file: " << path << std::endl;
        return 1;
    }

    std::string text;
    while (std::getline(file, text)) {
        std::cout << text << std::endl;
    }

    return 0;
}
