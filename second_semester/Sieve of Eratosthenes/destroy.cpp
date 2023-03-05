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
	int shmid = shmget(key, 300 * sizeof(int), 0644);
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}
