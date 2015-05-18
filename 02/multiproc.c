#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void child(int childId) {
	sleep(1);
	printf("child %d\n", childId);
    printf("child pid: %d, parent pid: %d\n", getpid(), getppid()); //pid = %d, ppid = %d\n""))"")
}

int main()
{
	const int NUM_CHILDREN = 100;
	pid_t pid;
	int childId;

    int status[NUM_CHILDREN];
    
	//vytvorenie detskych procesov
	for(childId = 0; childId < NUM_CHILDREN; childId++) {
			//child(childId);

            pid = fork();

            switch (pid) {
                case -1:
                    perror("fork");
                    exit(EXIT_FAILURE);
                case 0:
                    // child process
                    child(childId);
                    exit(EXIT_SUCCESS);
                default:
                    // ak som rodic tak preskoc
                    break;
            }
	}

	//pockanie na ukoncenie detskych procesov
	for(childId = 0; childId < NUM_CHILDREN; childId++) {
	    wait(&status[childId]);
	}
	printf("detske procesy ukoncene\n");

	exit(EXIT_SUCCESS);
}
