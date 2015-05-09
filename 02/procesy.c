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

//detsky proces
void child()
{   
    printf("I am in child\n");
    printf("child pid: %d, parent pid: %d\n", getpid(), getppid()); //pid = %d, ppid = %d\n"
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
    const int data_max_length = 10; // citanie po 10 znakoch
    char data[data_max_length + 1]; 
    ssize_t data_length;

    path = "udaje.txt";
    fd = open(path, O_WRONLY, mode);
    if (fd == -1) {
        perror("File open problem");
        exit(EXIT_FAILURE);
    }   

    printf("ok som tu");
    while ( (data_length = read(fd, data, data_max_length)) > 0 ) {
        data[data_length] = '\0';
        printf("parent %s", data);
        sleep(1);
    }   
    printf("length: %s", data);

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
