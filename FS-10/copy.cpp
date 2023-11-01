#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sstream>
#include <cstdlib>


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Please provide the source and destination file paths" << std::endl;
        return 1;
    }

    const char* sourcePath = argv[1];
    const char* destinationPath = argv[2];

    int sourceFile = open(sourcePath, O_RDONLY);

    if (sourceFile < 0) {
        std::cerr << "No such file : " << sourcePath << std::endl;
        exit(1);
    }


    struct stat openStat;
    fstat(destinationFile, &openStat);
    off_t fileSize = openStat.st_size;


    int destinationFile = open(destinationPath, O_WRONLY | O_CREAT);
    if (destinationFile < 0) {
        std::cerr << "No such file : " << destinationPath << std::endl;
        exit(1);
    }


    char buffer[512];
    int dataBytesCopied = 0;
    int holeBytesCopied = 0;

    int readBytesCount = read(sourceFile, buffer, fileSize);

    while (readBytesCount > 0) {
        std::cout << buffer;
        for (int i = 0; i < readBytesCount; ++i) {
            if (buffer[i] == '\0') {
                holeBytesCopied++;
            }
            else {
                dataBytesCopied++;
            }
        }
        write(destinationFile, buffer, readBytesCount);
        readBytesCount = read(sourceFile, buffer, fileSize);
    }

    close(sourceFile);
    close(destinationFile);

    std::cout << "Successfully copied " << dataBytesCopied + holeBytesCopied
        << " bytes (data: " << dataBytesCopied
        << ", hole: " << holeBytesCopied << ")." << std::endl;

    return 0;
}