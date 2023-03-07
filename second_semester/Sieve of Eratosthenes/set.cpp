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

	int shmid = shmget(key, 0, 0644);
	if(shmid < 0) {
        std::cout << "ERROR: Cannot open shared mem segment!!\n";	
        exit(1);
	}

    char *arr = (char *)shmat(shmid, NULL, 0);

	if (arr == (char*)-1) {
        perror("shmat");
        exit(1);
    }

	// creating sieve
    for (int i = 2; i * i <= 300; ++i) {
        if (arr[i] == 1) {
            for (int j = i*i; j <= 300; j += i) {
                arr[j] = 0;
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