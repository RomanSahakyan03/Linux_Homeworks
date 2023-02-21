#include <iostream>
#include <pthread.h>
#include <string>

struct args_t{
    std::string *src;
    std::string *dest;
    pthread_t thread;
    std::size_t from;
    std::size_t size;
};

void *copy_string(void *thread_args){
    args_t *args = (args_t *)thread_args;
    for (std::size_t i = args->from; i < args->from + args->size; ++i) {
      args->dest->at(i) = args->src->at(i);
    }
    //args->dest->replace(args->from, args->size, args->src->substr(args->from, args->size));
    return NULL;
}

int main() {
    std::string src;
    std::getline(std::cin, src);
    std::string dest(src.size(), ' ');
    
    std::size_t partitions = src.size() / 4;
    std::size_t remaining = src.size() % 4;
    pthread_t *threads = new pthread_t[4];


    for(int i = 0; i < 4; ++i){
        args_t *args = new args_t;
        args->src = &src;
        args->dest = &dest;
        args->from = i*partitions;
        args->size = partitions;

        if(i == 3 && remaining > 0){
            args->size += remaining;
        }

        pthread_create(&threads[i], NULL, copy_string, (void *)args);
    }

    for(int i = 0; i < 4; ++i){
        pthread_join(threads[i], NULL);
    }

    std::cout << dest << std::endl;
    delete[] threads;

    return 0;
}
