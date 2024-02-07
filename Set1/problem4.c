/*
 Write a complete, user-friendly, well-documented and nicely-indented C program which when executed as "./a.out executable1 executable2 ... executableN" creates N child processes where the 1st child process  executes the 1st executable file (given by executable1). 2nd executes the 2nd executable file (given by executable2) and so on. 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void executeChild(int index, char *executable, char *const argv[]) {
    printf("Child %d executing: %s\n", index + 1, executable);
    
    // Execute the given executable
    execve(executable, argv, NULL);
    
    // If execve fails
    perror("Exec failed");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    // Check if at least one argument (excluding program name) is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s executable1 executable2 ... executableN\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int numExecutables = argc - 1;

    // Create N child processes
    for (int i = 0; i < numExecutables; ++i) {
        pid_t childPid = fork();

        if (childPid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (childPid == 0) {
            // Child process
            char *const childArgs[] = { argv[i + 1], NULL };
            executeChild(i, argv[i + 1], childArgs);
        } else {
            // Parent process
            // Wait for the child to complete
            wait(NULL);
        }
    }

    printf("All child processes completed.\n");

    return 0;
}

