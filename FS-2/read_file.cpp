#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Please provide the file path"  << std::endl;
        return 1;
    }

    const char* source_file = argv[1];
    const char* destination_file = argv[2];

    std::ifstream source(source_file);
    if (!source) {
        std::cerr << "No such file: " << source_file << std::endl;
        return 1;
    }

    std::ofstream destination(destination_file);
    if (!destination) {
        std::cerr << "No such file: " << destination_file << std::endl;
        return 1;
    }

    std::string text;
    while (std::getline(source, text)) {
        destination << text << std::endl;
    }

    return 0;
}
