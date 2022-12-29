#ifndef PARALLEL_SCHEDULER
#define PARALLEL_SCHEDULER

#include <queue>
#include <pthread.h>
#include <string>
#include <mutex>
#include <vector>
#include <function>

class parallel_scheduler{
	private:
		std::queue<std:pair<std::function<void *()>, std::vector<std::string> > jobs;
		std::vector<pthread_t *> threads;

	public:
		parallel_scheduler(std::size_t n);

		~parallel_scheduler();

		void run();

		//void add_thread(std::size_t n = 1);

		//void delete_thread(std::size_t n = 1);

};

#endif