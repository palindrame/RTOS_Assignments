/*
 In the previous assignment you have written a C program which when executed as "./a.out executable1 executable2 ... executableN" creates N additional child processes where the 1st child process  executes the 1st executable file (given by executable1). 2nd executes the 2nd executable file (given by executable2) and so on.
In this assignment modify the program so that the parent process waits for completion of its N child processes and prints the status with which the child processes terminated along with the process id of the corresponding child process. Observe and report the status reported by a child process that terminates normally and one that terminates abnormally. Nnote that the first (least signficant) 8 bits of the status "reported" by a terminated child process are used by the OS (kernel) to signify abnormal termination!
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
		}
	}

	// Parent process
	int status;
	pid_t terminatedChild;

	// Wait for the completion of each child process and print their status
	for (int i = 0; i < numExecutables; ++i) {
		terminatedChild = wait(&status);

		if (status == 0) {
			printf("Child process %d (PID %d) terminated normally\n", i + 1, terminatedChild);
		} else {
			printf("Child process %d (PID %d) terminated abnormally with status: %d\n", i + 1, terminatedChild, WEXITSTATUS(status));
		}
	}

	printf("All child processes completed.\n");

	return 0;
}

