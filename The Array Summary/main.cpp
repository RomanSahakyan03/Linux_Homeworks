#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>


struct args_for_sum_t{

	int *arr;
	int begin;
	int end;
};


void *sum(void *args){

	args_for_sum_t *local_sum = (args_for_sum_t *) args;

	int * res = new int{0};

	for(int i = local_sum->begin; i < local_sum->end; ++i){

		*res += local_sum->arr[i];

	}

	delete local_sum;

	return res;

}


int main(int argc, char ** argv){

	if(argc < 3){
		std::cout << "Not enough arguments" << std::endl;
		return 0;
	}

	int array_size = std::atoi(argv[1]);
	int amount_of_threads = std::atoi(argv[2]);

	int *arr = new int[array_size];

	int result{};

	srand(std::time(0));

	for(int i = 0; i < array_size; ++i){

		arr[i] = rand()%10000;

	}
	auto time_begin_wo_threads = std::chrono::high_resolution_clock::now();


	for(int i = 0; i < array_size; ++i){
		result += arr[i];
	}


	auto time_end_wo_threads = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> time_span1 = time_end_wo_threads - time_begin_wo_threads;



	std::cout << "Time spent without threads: "<< time_span1.count() << std::endl;

	result = 0;




	pthread_t *threads = new pthread_t[amount_of_threads];

	for(int i = 0; i < amount_of_threads; ++i){

		args_for_sum_t *sub_args = new args_for_sum_t;

		sub_args->arr = arr;
		sub_args->begin = i*(array_size / amount_of_threads);
		sub_args->end = sub_args->begin + (array_size / amount_of_threads);


		int result = pthread_create(&threads[i], NULL, sum, sub_args);

		if(result < 0) exit(result);

	}


	auto time_begin_w_threads = std::chrono::high_resolution_clock::now();

	for(int i = 0; i < amount_of_threads; ++i){

		void *summarize;
		int finishing_thread = pthread_join(threads[i], &summarize);

		if(finishing_thread < 0) exit(finishing_thread);


		int *sub_sum = (int *) summarize;
		result += *sub_sum;

		delete sub_sum;

	}

	auto time_end_w_threads = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> time_span2 = time_end_w_threads - time_begin_w_threads;

	delete [] arr;
	delete [] threads;

	std::cout << "Time spent with threads: "<< time_span2.count() << std::endl;

	return 0;

}