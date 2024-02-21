#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H

#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <string.h>


class shared_array {
private:

    int fd ;
    const char* array_name;
    int size;
    int* array;

public:

  shared_array(std::string _array_name, int _size);

  bool operator==(const shared_array &other);

  int& operator[](int index);
};
#endif // SHARED_ARRAY_H