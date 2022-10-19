
all: 	main.o complex.o
	g++ main.o complex.o -o main

main.o: main.cpp
	g++ -c main.cpp

complex.o: complex.cpp complex.hpp
	g++ -c complex.cpp

clean:
	rm *.o main
