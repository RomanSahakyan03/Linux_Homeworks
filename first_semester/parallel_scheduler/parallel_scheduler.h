#ifndef PARALLEL_SCHEDULER
#define PARALLEL_SCHEDULER

#include <queue>
#include <pthread.h>
#include <string>
#include <mutex>
#include <vector>
#include <functional>
#include <any>

class parallel_scheduler{
	private:
		std::vector<pthread_t> threads;
		std::queue<std::tuple<std::function<std::any()>, std::tuple<>>> jobs;

	public:
		parallel_scheduler(std::size_t n = 1);

		~parallel_scheduler();

		void run();

		void *working(void *arg);

		//void add_thread(std::size_t n = 1);

		//void delete_thread(std::size_t n = 1);

};

#endif