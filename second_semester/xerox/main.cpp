#include <pthread.h>
#include <string>
#include <iostream>
#include <unistd.h>

#define THREADS_SIZE 20

pthread_cond_t cv = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t mt = PTHREAD_MUTEX_INITIALIZER; 

struct printer_busy{
  int i;
  bool is_free = true;
};

void *printer_usage(void * args){
    printer_busy *xerox = (printer_busy *)args;
    while(true){
        pthread_mutex_lock(&mt);
        
        while(xerox->is_free != true){
          pthread_cond_wait(&cv, &mt);
        } 
       
        xerox->is_free = false;
        std::cout << xerox->i << std::endl;
        pthread_cond_broadcast(&cv);
        sleep(0.5);
        xerox->is_free = true;


        pthread_mutex_unlock(&mt);
    }

    delete xerox;
    return nullptr;
}


int main(){
    pthread_t threads[THREADS_SIZE];
    for(int i = 0; i < THREADS_SIZE; ++i){
        printer_busy *args = new printer_busy;
        args->i = i;
        pthread_create(&threads[i], nullptr, printer_usage, (void *)args);
    }

    for(int i = 0; i < THREADS_SIZE; ++i){
        pthread_join(threads[i], nullptr);
    }
    return 0;
}