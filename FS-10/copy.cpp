#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sstream>
#include <cstdlib>
#include <cerrno>
#include <cmath>
#include <limits>


int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cerr << "Please provide the source and destination file paths" << std::endl;
        return 1;
    }

    const char* sourcePath = argv[1];
    const char* destinationPath = argv[2];

    //Open source file
    int sourceFile = open(sourcePath, O_RDONLY);
    if (sourceFile < 0) {
        std::cerr << "No such file : " << sourcePath << std::endl;
        exit(1);
    }

    //Open destination file
    int destinationFile = open(destinationPath, O_WRONLY);
    if (destinationFile < 0) {
        std::cerr << "No such file : " << destinationPath << std::endl;
        exit(1);
    }

    //clear the file
    ftruncate(destinationFile, 0);

    //buffer
    int const bufferSize = 512;
    char* buffer = new char[bufferSize];

    //counters
    int dataBytesCopied = 0;
    int holeBytesCopied = 0;

    //pointer
    int last = 0;



    // go over file
    while (true) {

        // try jump to next beginning of hole
        off_t data = lseek(sourceFile, last, SEEK_HOLE);        

        // there was some data if we got to next hole
        if (data > 0) {

            //update counter
            dataBytesCopied += data - last;

            //read the data
            lseek(sourceFile, last, SEEK_SET);
            while (true) {

                int bytesToWrite = std::min((int)data - last, bufferSize);

                read(sourceFile, buffer, bytesToWrite);

                if (bytesToWrite == 0) {
                    break;
                }

                lseek(destinationFile, last, SEEK_SET);
                write(destinationFile, buffer, bytesToWrite);

                last += bytesToWrite;

            }

        }


        // reached the end of file
        if (data == 0 || (data == -1 && errno == ENXIO)) {
            break;
        }

        // something went wrong
        if (data < 0) {
            std::cerr << "Something went wrong1. " << strerror(errno) << std::endl;
            exit(errno);
        }

        // try find next data jumping over a hole
        off_t hole = lseek(sourceFile, last, SEEK_DATA);

        // reached the end no data to read
        if (hole == -1 && errno == ENXIO) {
            break;
        }

        // something went wrong
        if (hole == -1) {
            std::cerr << "Something went wrong. " << strerror(errno) << std::endl;
            exit(errno);
        }

        if (hole > 0) {
            //create hole
            lseek(destinationFile, hole - last, SEEK_END);

            //update counter
            holeBytesCopied += hole - last;

            // move pointer
            last = hole;
        }
    }

    //close the files
    close(sourceFile);
    close(destinationFile);

    std::cout << "Successfully copied " << last
        << " bytes (data: " << dataBytesCopied
        << ", hole: " << holeBytesCopied << ")." << std::endl;

    return 0;
}