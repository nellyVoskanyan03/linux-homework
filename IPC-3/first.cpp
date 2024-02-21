#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <string.h>
#include "shared_array.h"
#include <semaphore.h>

const char *semaphore = "/sem";


int main(int argc, char** argv) {


      int size = 23;
      shared_array array("arr", size);
      array[4] = 7;
      array[5]=6;
      std::cout << array[4] << std::endl;
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
     while(true) {
       
            sem_wait(sem);
            std::cout << "writing ";
            array[rand() % size]++;
            sem_post(sem);
            std::this_thread::sleep_for(std::chrono::microseconds(50000));
     }
     sem_close(sem);
     sem_unlink(semaphore);

  
      return 0;
}
