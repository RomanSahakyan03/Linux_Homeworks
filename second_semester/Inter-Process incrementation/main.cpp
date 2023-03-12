#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHM_SIZE  sizeof(int)
#define NUM_ITERATIONS 10000

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char  **argv) {
    int pid1, pid2;
    int EXIT_STATUS1, EXIT_STATUS2;
    key_t key, shm_key;
    int semid, semval;
    int shmid;
    struct sembuf semops;
    union semun semarg;
    int *integer;

    // create a unique key for the semaphore set
    if ((key = ftok(".", 'S')) == -1) {
        perror("ftok");
    }

    // create a semaphore set with 1 semaphore
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
    }

    // initialize the semaphore value to 1
    semarg.val = 1;
    if (semctl(semid, 0, SETVAL, semarg) == -1) {
        perror("semctl");
    }

    // create a unique key for the shared memory
    if ((shm_key = ftok(".", 'M')) == -1) {
        perror("ftok");
    }

    // create the shared memory segment
    if ((shmid = shmget(shm_key, SHM_SIZE, 0666 | IPC_CREAT)) == -1) {
        if (errno != EEXIST) {
			std::cout << "ERROR: Cannot create shared mem segment!!\n";	
			exit(1);
		}
		else {
			shmid = shmget(shm_key, SHM_SIZE, 0666);
		}
    }
    

    // attach to the shared memory segment
    if ((integer = (int *) shmat(shmid, NULL, 0)) == (int *) -1) {
        perror("shmat");
    }

    (pid1 = fork()) && (pid2 = fork());

    if(pid1 < 0 || pid2 < 0){
        perror("forking error");
    }

    if(pid1 > 0 && pid2 > 0){
        wait(&EXIT_STATUS1);
        wait(&EXIT_STATUS2);

        

    }else if(pid1 == 0){

        // Attach to the semaphore
        int semid_child = semget(key, 1, 0666);
        if (semid_child == -1) {
            perror("semget failed");
            exit(EXIT_FAILURE);
        }
        if (semctl(semid_child, 0, SETVAL, semarg) == -1) {
            perror("semctl failed");
            exit(EXIT_FAILURE);
        }

        // Wait for the semaphore to become available
        semops.sem_num = 0;
        semops.sem_op = -1;
        semops.sem_flg = 0;
        if (semop(semid_child, &semops, 1) == -1) {
            perror("semop failed");
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < NUM_ITERATIONS; ++i){
            ++(*integer);
        }

        // Release the semaphore
        semops.sem_op = 1;
        if (semop(semid_child, &semops, 1) == -1) {
            perror("semop failed");
            exit(EXIT_FAILURE);
        }

        // Detach from the semaphore
        if (semctl(semid_child, 0, IPC_RMID) == -1) {
            perror("semctl failed");
            exit(EXIT_FAILURE);
        }

        return 0;

    }else if(pid2 == 0){
        // Attach to the semaphore
        int semid_child = semget(key, 1, 0666);
        if (semid_child == -1) {
            perror("semget failed");
            exit(EXIT_FAILURE);
        }
        if (semctl(semid_child, 0, SETVAL, semarg) == -1) {
            perror("semctl failed");
            exit(EXIT_FAILURE);
        }

        // Wait for the semaphore to become available
        semops.sem_num = 0;
        semops.sem_op = -1;
        semops.sem_flg = 0;
        if (semop(semid_child, &semops, 1) == -1) {
            perror("semop failed");
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < NUM_ITERATIONS; ++i){
            ++(*integer);
        }

        // Release the semaphore
        semops.sem_op = 1;
        if (semop(semid_child, &semops, 1) == -1) {
            perror("semop failed");
            exit(EXIT_FAILURE);
        }

        // Detach from the semaphore
        if (semctl(semid_child, 0, IPC_RMID) == -1) {
            perror("semctl failed");
            exit(EXIT_FAILURE);
        }

        return 0;
    }

    std::cout << *integer << std::endl;

    if(shmdt(integer) == -1){
		perror("shmdt");
        exit(1);
	}

	if(shmctl(shmid, IPC_RMID, NULL) < 0){
		perror("shmctl");
	}

    // remove the semaphore set
    if (semctl(semid, 0, IPC_RMID, semarg) == -1) {
        perror("semctl");
    }

    return 0;
}