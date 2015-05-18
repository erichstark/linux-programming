#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int child() 
{
	srand(time(NULL));
	int returnCode = rand()%10;
	printf("child: returnCode = %d\n", returnCode);
	return returnCode;
}

void parent()
{
    int result;
    wait(&result);
	
    if (WIFEXITED(result)) {
        printf("Child process exited normally.\nReturn code from child: %d\n", WEXITSTATUS(result));
    } else {
        printf("Child process exited with error.\n")
    }

    //pockaj na ukoncenie dietata a zisti status
	//ak bolo dieta normalne ukoncene
	//tak vypis jeho navratovy kod
}

int main()
{
	pid_t pid;
	int childReturnCode;

	pid = fork();
	if( pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if( pid == 0 ) {
		childReturnCode = child();
		exit(childReturnCode);
	}
	else {
		parent();
		exit(EXIT_SUCCESS);
	}
}
	
