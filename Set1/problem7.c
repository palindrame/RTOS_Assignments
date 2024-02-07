/*
 In the previous assignment you have written a C program which when executed as "./a.out executable1 executable2 ... executableN" creates N additional child processes where the 1st child process  executes the 1st executable file (given by executable1). 2nd executes the 2nd executable file (given by executable2) and so on.
In this assignment write a program so that the parent process waits for completion of its  child processes and uses the status with which the child processes terminated.
The parent process read 2 matrices  A (size m X n)  and B (size  n X r) and creates m*r child processes such that  each child process will compute one element of the product matrix A X B (size m*r) and communicates that element as status to the parent process. The parent process finally prints the product matrix (A X B)!
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int m, n, r;

    // Input matrix A
    printf("Enter the number of rows and columns for matrix A (m n): ");
    scanf("%d %d", &m, &n);
    int A[m][n];
    printf("Enter the elements of matrix A:\n");
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            scanf("%d", &A[i][j]);
        }
    }

    // Input matrix B
    printf("Enter the number of rows and columns for matrix B (n r): ");
    scanf("%d %d", &n, &r);
    int B[n][r];
    printf("Enter the elements of matrix B:\n");
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < r; ++j) {
            scanf("%d", &B[i][j]);
        }
    }

    // Result matrix
    int result[m][r];

    // Fork m*r child processes
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < r; ++j) {
            pid_t pid = fork();

            if (pid == 0) {
                // Child process
                int element = 0;
                for (int k = 0; k < n; ++k) {
                    element += A[i][k] * B[k][j];
                }

                // Communicate the result to the parent using exit status
                exit(element);
            } else if (pid < 0) {
                // Error handling
                perror("Fork failed");
                exit(EXIT_FAILURE);
            }
            // Parent continues to fork next child
        }
    }

    // Parent waits for all child processes to complete
    for (int i = 0; i < m * r; ++i) {
        int status;
        wait(&status);
        result[i / r][i % r] = WEXITSTATUS(status);
    }

    // Print the product matrix
    printf("Product Matrix (A X B):\n");
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < r; ++j) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}

