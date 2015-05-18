#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    
    //for (int i = 0; i < argc; i++) {
    //    printf("%s\n", argv[i]);
    //}

	execvp(argv[1], argv);
	//chybove hlasenie
    perror("exec");

	return EXIT_FAILURE;
}
