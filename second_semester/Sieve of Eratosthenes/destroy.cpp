#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>


int main() {
	std::string pathname = "shm_file";
	
	// create a key for shared mem
	key_t key = ftok(pathname.c_str(), 'a');
	if(key < 0) {
		std::cout << "ERROR: Cannot generate a key!!\n";
		exit(errno);
	}
	int shmid = shmget(key, 0, 0644);
	if(shmid < 0) {
        std::cout << "ERROR: Cannot open shared mem segment!!\n";	
        exit(1);
	}

	
	int del = shmctl(shmid, IPC_RMID, NULL);

	if(del < 0){
		perror("shmctl");
	}
	return 0;
}
