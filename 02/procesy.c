#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

// opravnenia
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// wait
#include <sys/wait.h>

// pre vypis casu
#include <time.h>

// treba pre strlen
#include <string.h>

//detsky proces
void child()
{   
    printf("I am in child\n");
    printf("child pid: %d, parent pid: %d\n", getpid(), getppid()); //pid = %d, ppid = %d\n"
    
    time_t result = time(NULL);

    int fd;
    char *path;
    char *time = ctime(&result);
    int len = strlen(time);
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;

    path = "new.txt";

    fd = open(path, O_WRONLY, mode);
    if (fd == -1) {
        perror("open problem");
        exit(EXIT_FAILURE);
    }

    if (write(fd, time, len) != len)
        perror("write");
    if (close(fd) == -1)
        perror("close");

    //fprintf(stdout, "Aktualny cas je: %s", ctime(&result));
}

//rodicovsky proces
//vstup:
//  childPid - cislo detskeho procesu
void parent(pid_t childPid)
{
    int fd;
    char *path;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;

    printf("I am parent\n");
    printf("parent pid: %d, child pid: %d\n", getpid(), childPid); //pid = %d, childPid = %d\n"
    
    // for reading
    const int data_max_length = 30; // citanie po 10 znakoch
    char data[data_max_length + 1]; 
    ssize_t data_length;

    path = "new.txt";

    // mal som WRONLY a nedalo sa citat!!!
    fd = open(path, O_RDONLY, mode);
    // printf("open %d\n", fd);
    if (fd == -1) {
        perror("File open problem");
        exit(EXIT_FAILURE);
    }   

    while ( (data_length = read(fd, data, data_max_length)) > 0 ) {
        data[data_length] = '\0';
        printf("Datum je: %s\n", data);
    }   

    close(fd);
}

int main()
{
    int hod;
    pid_t pid;
    pid = fork();

    // caka na ukoncenie child
    wait(&hod);

    switch (pid) {
        case -1:
            perror("Problem with fork \n");
            exit(EXIT_FAILURE);
            break;
        case 0:
            child();
            exit(EXIT_SUCCESS);
            break;
        default:
            parent(pid);
            exit(EXIT_SUCCESS);
            break;
    }
}
