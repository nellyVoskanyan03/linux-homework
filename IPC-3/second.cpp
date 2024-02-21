#include "shared_array.h"
#include <semaphore.h>
#include <chrono>
#include <iostream>
#include <thread>

const char *semaphore = "/sem";
int main(int argc, char** argv) {

  
  int size = 23;
    shared_array array("arr", size);
    for(int i = 0; i < size; ++i){
            array[i] = i;
    }
    sem_t *sem = sem_open(semaphore, O_CREAT | O_EXCL, 0644, 1);
    if (sem == SEM_FAILED) {
        if(errno == EEXIST)
        {
            int sem_un_res = sem_unlink(semaphore);
            if(sem_un_res < 0)
            {
                std::cerr << strerror(errno) << std::endl;
                exit(errno);
            }

            sem = sem_open(semaphore, O_CREAT | O_EXCL, 0644, 1);
        }
        else{
            std::cerr << strerror(errno) << std::endl;
            exit(errno);
        }
    }

    while(true){
      sem_wait(sem);
        for(int i = 0; i < size; ++i){
            std::cout << array[i] << std::endl;
        }
      sem_post(sem);
      std::this_thread::sleep_for(std::chrono::microseconds(40000));
    }
    sem_close(sem);
    sem_unlink(semaphore);

  return 0;
}
