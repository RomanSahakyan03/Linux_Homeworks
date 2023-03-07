#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>

int main(){
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

    char *arr = shmat(shmid, NULL, 0);

	// creating resheto
    for (int i = 2; i <= N; i++) {
        if (arr[i-1] == 0) {
            for (int j = i*i; j <= N; j += i) {
                arr[j-1] = 1;
            }
        }
    }

	// unlink
    if (shmdt(arr) == -1) {
        perror("shmdt");
        exit(1);
    }
    
    return 0;
}