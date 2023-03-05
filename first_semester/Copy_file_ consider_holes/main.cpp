#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <unistd.h>
#include <cstring>

#define BUFFER_SIZE 4096

//function that returns logical size of the file
int logical_size(int fd){
	int logic_size = 0;
	
	//bring the offset to the beginning
	logic_size = lseek(fd, 0, SEEK_SET);

	//set the offset at the end
	logic_size = lseek(fd, 0, SEEK_END);

	//check for errors
	if(logic_size < 0){
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return errno;
	}

	//bring offset back to the beginning for further steps	
	lseek(fd, 0, SEEK_SET);
	return logic_size;
}

//function that returns physical size of the file
int physical_size(int fd){
	int phys_size = 0;

	//bring the offset to the beginning
	int pos = lseek(fd, 0, SEEK_SET);
	int start = 0;
	int end = 0;
	int data_size = 0;

	//until the offset has reached the end
	while(pos != lseek(fd, 0, SEEK_END)){

		//set the offset to the nearest data
		pos = lseek(fd, 0, SEEK_DATA);

		//check for errors
		if(pos < 0){
			std::cerr << "Error: " << strerror(errno) << std::endl;
			return errno;
		}

		//save the start of the data
		start = pos;

		//set the offset to the nearest hole (where the data ends) 
		pos = lseek(fd, 0, SEEK_HOLE);

		//check for errors
		if(pos < 0){
			std::cerr << "Error: " << strerror(errno) << std::endl;
			return errno;
		}

		//save the next pos of the end of the data
		end = pos;

		//difference will give the data size
		data_size = end - start;

		//add to phys_size
		phys_size += data_size;
	}
	lseek(fd, 0, SEEK_SET);
	return phys_size;
}

int main(int argc, const char** argv){

	// check if path arguments are given
	if(argc < 3){
		std::cerr << "Lack of arguments" << std::endl;
		return 1;
	}

	//get the source path from the arguments
	const char* source_path = argv[1];

	//get the destination path from the arguments
	const char* dest_path = argv[2];

	//open source file in read_only mode
	int source_fd = open(source_path, O_RDONLY);

	//check if the file is open
	if(source_fd < 0){
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return errno;
	}

	//open the destination file in creat, write_only and trunc modes
	int dest_fd = open(dest_path, O_CREAT | O_TRUNC | O_WRONLY, 0644);

	//check if the file is open
	if(dest_fd < 0){
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return errno;
	}
	std::cout << "Source file" << std::endl;
	std::cout << std::endl;
	//print the logical size of the source file
	std::cout << "Logical size: " << logical_size(source_fd)<< " bytes" << std::endl;

	//print the physical size of the source file
	std::cout << "Physical size: " << physical_size(source_fd) << " bytes" << std::endl;
	std::cout << std::endl;	
	//creat buffer
	char* buffer = new char[BUFFER_SIZE];

	//copy the source file to the destination file
	while(true){

		//read from source file into the buffer
		int read_bytes = read(source_fd, buffer, BUFFER_SIZE);

		//no more data to read
		if(read_bytes == 0){
			break;
		}

		//check for errors
		if(read_bytes < 0){
			std::cerr << "Error: " << strerror(errno) << std::endl;
			return errno;
		}

		//write up from the buffer to the destination file
		int write_bytes = write(dest_fd, buffer, read_bytes);

		//check for errors
		if(write_bytes < 0){
			std::cerr << "Error: " << strerror(errno) << std::endl;
			return errno;
		}
		
		//no more data to write
		if(write_bytes == 0){
			break;
		}
	}
	std::cout << "Destination file" << std::endl;
	std::cout << std::endl;
	//print the logical size of the source file
	std::cout << "Logical size: " << logical_size(dest_fd) << " bytes" << std::endl;

	//print the physical size of the destination file
	std::cout << "Physical size: " << physical_size(dest_fd) << " bytes" << std::endl;

	//delete the buffer
	delete [] buffer;

	//close the source file
	close(source_fd);

	//close the destination file
	close(dest_fd);
	return 0;
}