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

	// create shared mem segment
	int shmid = shmget(key, 300 * sizeof(char), 0644 | IPC_CREAT | IPC_EXCL);
	if(shmid < 0) {
		if (errno != EEXIST) {
			std::cout << "ERROR: Cannot create shared mem segment!!\n";	
			exit(1);
		}
		else {
			shmid = shmget(key, 300 * sizeof(char), 0644);
		}
	}

    char *arr = (char *)shmat(shmid, NULL, 0);

    for(int i = 0; i < 300; ++i){
        arr[i] = 0;
    }

	if(shmdt(arr) == -1){
		perror("shmdt");
		exit(errno);
	}

    return 0;
}