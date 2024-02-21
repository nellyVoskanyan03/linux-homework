#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <string.h>
#include "shared_array.h"

# define PAGE_SIZE 4096

shared_array::shared_array(std::string _array_name, int _size): size(_size) { 
    // create shared file
    this->array_name = (_array_name + "_" + std::to_string(size)).c_str();
    
    int fd = shm_open(this->array_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (fd < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(1);
    }

    int ft = ftruncate(fd, size);

    if (fd < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(1);
    }
    int page_count = this->size / PAGE_SIZE;
    if(size % PAGE_SIZE != 0){
        page_count++;
    }
    
    this->array = (int*)mmap(nullptr, 
                    page_count * PAGE_SIZE, 
                    PROT_WRITE | PROT_READ, 
                    MAP_SHARED, 
                    fd, 
                    0);


}

bool shared_array::operator==(const shared_array &other) {
    return this->array_name == other.array_name && this->size == other.size;
}

int& shared_array::operator[](int index) {
    //get element from shared memory
    return this->array[index];
}

shared_array::~shared_array() {
    munmap(this_array, 4096);
    close(fd);
}

