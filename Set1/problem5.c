/*
 In the previous assignment we have developed a C program which when executed as "./a.out executable1 executable2 ... executableN" creates N child processes where the 1st child process 1st executes the 1st executable file (given by executable1). 2nd executes the 2nd executable file (given by executable2) and so on.

Please note that in this assignment no scope was there to pass command line arguments to the executables. Extend the assignment so that this can be done now. That is the program now can be called as "./a.out ls -l ps -ef" such that 1st child process executes the command "ls -l" and the second one executes "ps -ef"!

Please note that "ls" or "ps" are executable files in "/bin" folder. That is the absolute path for those files are "/bin/ls" and "/bin/ps". That is, the command line for testing for your program could be "./a.out /bin/ls -l /bin/ps -ef", as well.

Also please note that, here, a.out will receive 4 command line arguments ("/bin/ls", "-l", "/bin/ps", "-ef", argc being 5). Whereas, "ls" and "ps"  will be executed with 1 command line argument, "-l" and "-ef", respectevily.

"ls" and  "ps" have been  mentioned here just as examples. Your program should work for any executable file for child processes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void executeChild(int index, char *executable, char *const argv[]) {
    printf("Child %d executing: %s\n", index + 1, executable);
    
    // Execute the given executable with command-line arguments
    execve(executable, argv, NULL);
    
    // If execve fails
    perror("Exec failed");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    // Check if at least one argument (excluding program name) is provided
    if (argc < 3) {
        fprintf(stderr, "Usage: %s executable1 arg1 arg2 ... executableN arg1 arg2 ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int numExecutables = (argc - 1) / 2;

    // Create N child processes
    for (int i = 0; i < numExecutables; ++i) {
        pid_t childPid = fork();

        if (childPid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (childPid == 0) {
            // Child process
            int argIndex = 2 * i + 1; // Calculate the index of the first argument for the current executable
            char *const childArgs[] = { argv[argIndex], argv[argIndex + 1], NULL };
            executeChild(i, argv[argIndex], childArgs);
        } else {
            // Parent process
            // Wait for the child to complete
            wait(NULL);
        }
    }

    printf("All child processes completed.\n");

    return 0;
}

