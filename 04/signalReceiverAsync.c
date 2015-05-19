#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

//doplnte funkciu SignalHandler
void SignalHandler(int signal)
{
    printf("Signal: %d\n", signal);
}

int main()
{
    struct sigaction akcia;
    //doplnte vypis PID
    printf("PID procesu: %d\n", getpid());

    //doplnte asynchronne spracovanie signalov

	akcia.sa_handler = &SignalHandler;
    sigaction(SIGUSR1, &akcia, NULL);
    sigaction(SIGINT, &akcia, NULL);

	while(1) {
		printf(".");
		fflush(stdout);
		sleep(1);
	}
	
	return EXIT_SUCCESS;
}

 
