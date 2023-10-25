#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Please provide the file path " << std::endl;
        return 1;
    }

    const char* file_to_erase = argv[1];

    struct stat file_stat;
    if (stat(file_to_erase, &file_stat) != 0) {
        std::cerr << "File not found: " << file_to_erase << std::endl;
        return 1;
    }

    int file_descriptor = open(file_to_erase, O_WRONLY);
    if (file_descriptor == -1) {
        std::cerr << "No such file: " << file_to_erase << std::endl;
        return 1;
    }

    off_t size = file_stat.st_size;

    
    char null_buffer[1024] = { 0 };
    while (file_size > 0) {
        size_t bytes_to_write = std::min(static_cast<size_t>(file_size), sizeof(null_buffer));
        if (write(file_descriptor, null_buffer, bytes_to_write) == -1) {
            std::cerr << "Error overwriting file: " << file_to_erase << std::endl;
            close(file_descriptor);
            return 1;
        }
        file_size -= bytes_to_write;
    }

    close(file_descriptor);

    unlink(file_to_erase);

    return 0;
}
