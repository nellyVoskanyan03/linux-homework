#include <iostream>
#include <dlfcn.h>
#include "parallel_scheduler.cpp"
#include "parallel_scheduler.h"

void func1 (void* arg){
    std::cout << "runing func1" << std::endl;
}

void func2 (void* arg){
    std::cout << "runing func2 wthh argument: " << arg << std::endl;
}

void func3 (void* arg){
    std::cout << "runing func3" << std::endl;
}


int main() {
    
    void* shared_library = dlopen("./parallel_scheduler.so", RTLD_LAZY);
    if (!shared_library) {
        std::cerr << "Error loading the library " << std::endl;
        return 1;
    }

    // Create an instance of parallel_scheduler from the shared library
    parallel_scheduler* (*create_parallel_scheduler)(size_t capacity);
    create_parallel_scheduler = (parallel_scheduler* (*)(size_t)) dlsym(shared_library, "create_parallel_scheduler");

    if (!create_parallel_scheduler) {
        std::cerr << "Error creating instance of parallel_scheduler " << dlerror() << std::endl;
        return 1;
    }

    size_t capacity = 4;
    parallel_scheduler* scheduler = (parallel_scheduler*)create_parallel_scheduler(capacity);

    
    for (int i = 0; i < 10; ++i) {
        scheduler->run(&func1, nullptr);
        scheduler->run(&func2, new int(i));
        scheduler->run(&func3, nullptr);
    }

    // Unload the shared library
    dlclose(shared_library);

    return 0;
}
