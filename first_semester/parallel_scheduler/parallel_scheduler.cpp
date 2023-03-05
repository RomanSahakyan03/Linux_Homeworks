#include "parallel_scheduler.h"

void *working(void *arg){
	
}

parallel_scheduler::parallel_scheduler(std::size_t n){

	threads.resize(n);

	for(auto &x: threads){

		int result = pthread_create(&x, NULL, parallel_scheduler::working, nullptr);

        	if (result < 0){
            		exit(result);
        	}
	}

}

parallel_scheduler::~parallel_scheduler(){

	for(auto x: threads){

		int result = pthread_join(x, NULL);

        	if (result < 0){
            		exit(result);
        	}

	}

}

void parallel_scheduler::run(){}