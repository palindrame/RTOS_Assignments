/*
Write a complete C program that reads 2 numbers  n1 and n2  (n1≤n2) as command line arguments. That is, uses "int argc" and "char *argv[]" to read  n1 and n2 when the program is executed as "./a.out n1 n2". The program then creates n2 - n1 + 1 child processes Pn1, Pn1+1, ..., Pn2 such that Pi, n1 ≤  i ≤  n2, computes and prints the factorial of i. That is Pn1 computes and prints the factorial of n1, Pn1+1 computes and prints the factorial of n1+1, and so on!
Additionally ensure that process Pi uses the value computed by Pi-1. That is, while computing i! (factorial of i) Pi uses  (i-1)! computed by Pi-1.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void calculateFactorial(int n, int pipe_fd[]) {
    close(pipe_fd[0]); // Close reading end in the child process

    int factorial = 1;
    for (int i = 1; i <= n; ++i) {
        factorial *= i;
    }

    write(pipe_fd[1], &factorial, sizeof(factorial)); // Write the result to the pipe
    close(pipe_fd[1]); // Close writing end in the child process
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <n1> <n2>\n", argv[0]);
        return 1;
    }

    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[2]);

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Child process
        calculateFactorial(n1, pipe_fd);
    } else {
        // Parent process
        int factorial = 1;
        close(pipe_fd[1]); // Close writing end in the parent process

        for (int i = n1; i <= n2; ++i) {
            wait(NULL); // Wait for the child to finish

            // Read the result from the pipe
            read(pipe_fd[0], &factorial, sizeof(factorial));

            printf("Factorial of %d: %d\n", i, factorial);

            // Calculate the next factorial and pass it to the child
            factorial *= (i + 1);
            write(pipe_fd[1], &factorial, sizeof(factorial));
        }

        close(pipe_fd[0]); // Close reading end in the parent process
    }

    return 0;
}




