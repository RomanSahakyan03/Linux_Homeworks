all:
	g++ main.cpp parallel_scheduler.o -pthread -o demo-application

parallel_scheduler.o: parallel_scheduler.cpp parallel_scheduler.h
	g++ -c parallel_scheduler.cpp parallel_scheduler.h -o parallel_scheduler.o

clean:
	rm *.o demo-application