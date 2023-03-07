#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv){
    if(argc < 2){
        std::cerr << "not enough arguments" << std::endl;
        return 0;
    }
    int n = std::atoi(argv[1]);

    if(n < 2 && n > 300){
        std::cerr << "input valid number (from 2 to 300)" << std::endl;
    }

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

    // attachinh the segment
    char *arr = (char *) shmat(shmid, nullptr, 0);
    if (arr == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    if (arr[n] != 0){
        std::cout << "Yes" << std::endl;
    }else{
        std::cout << "No" << std::endl;
    }

    // unlinking
    if (shmdt(arr) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}