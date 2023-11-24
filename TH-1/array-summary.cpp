#include <iostream>
#include <string>
#include <algorithm>
#include <string.h>
#include <chrono>
#include <sys/times.h>
#include <sys/types.h>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>

struct local_sum_arg {
    int* array;
    int start;
    int end;

};

void* local_sum(void* arg) {
    local_sum_arg* part = (local_sum_arg*)arg;

    int sum = 0;

    for (int i = part->start; i < part->end; i++) {
        sum += part->array[i];
    }

    return new int(sum);

}

int sum_with_multithreading(int threads_count, int* array, int size) {
    pthread_t* threads = new pthread_t[threads_count];
    local_sum_arg** array_of_args = new local_sum_arg * [threads_count];
    int sum = 0;
    for (int i = 0; i < threads_count; ++i)
    {
        array_of_args[i] = new local_sum_arg;
        array_of_args[i]->array = array;
        array_of_args[i]->start = i * (size / threads_count);
        array_of_args[i]->end = std::min(array_of_args[i]->start + (size / threads_count), size);
        int result = pthread_create(&threads[i], NULL, local_sum, array_of_args[i]);
        if (result != 0)
        {
            std::cerr << strerror(result) << std::endl;
            exit(result);
        }
    }


    for (int i = 0; i < threads_count; ++i)
    {
        void* return_value;
        int result = pthread_join(threads[i], &return_value);
        int* local_sum_result = (int*)return_value;
        sum += *local_sum_result;
        delete local_sum_result;
        delete array_of_args[i];
    }

    delete[] array_of_args;

    return sum;
    
}

int sum(int* array, int size) {
    int sum = 0;

    for (int i = 0; i < size; i++) {
        sum += array[i];
    }

    return sum;

}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Array size/Threads count is not given" << std::endl;
    }

    int size = std::stoi(argv[1]);
    int thread_count = std::stoi(argv[2]);
    int* array = new int[size];

    for (int i = 0; i < size; i++) {
        array[i] = 1;
    }

    struct tms start, end;
    clock_t start_real, end_real;

    // Time spent without threads
    times(&start);
    start_real = times(&start);

    sum(array, size);

    times(&end);
    end_real = times(&end);

    // Calculate the duration
    clock_t ticks_per_second = sysconf(_SC_CLK_TCK);
    double duration = (double)(end_real - start_real) / ticks_per_second;
    std::cout << "Time spent without threads: " << duration << std::endl;

   




    // Time spent with threads
    times(&start);
    start_real = times(&start);

    sum_with_multithreading(thread_count, array, size);
    
    times(&end);
    end_real = times(&end);

    // Calculate the duration
    duration = (double)(end_real - start_real) / ticks_per_second;
    std::cout << "Time spent with " << thread_count << " threads: " << duration << std::endl;

    delete[] array;

    return 0;
}