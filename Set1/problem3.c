/*A polynomial of degree n (in one variable, with real coefficients) is an expression of the form: anxn + an-1xn-1 + an-2xn-2 + ··· + a2x2 + a1x + a0 where an, an-1, an-2, ··· a2, a1, a0 are real numbers. Example: 3x4 - 2x2 + 5x + 1 is a polynomial of degree 4.

Write a complete C program that reads a polynomial of degree n  through command line arguments. That is, when the program is executed as "./a.out v  an an-1  an-2 ... a2 a1 a0", it uses "int argc" and "char *argv[]" to read an, an-1, an-2, ··· a2, a1, a0 and the value of x (say, v) for which the polynomial is to be evaluated.
The program then creates n+1 child processes P0, P1, P2, ..., Pn such that Pi, 1 ≤  i ≤  n, evaluates the ith term of the polynomial. That is P0 evaluates anvn ,P1 evaluates an-1vn-1 , and so on. Pn, in addition to evaluating the nth term (a0),  also prints the value of the whole polynomial for v.
Please note that your program should be well-documented and properly indented for easy reading! */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s v a_n a_{n-1} ... a_2 a_1 a_0\n", argv[0]);
        return 1;
    }

    int n = argc - 3; // Degree of the polynomial
    double v = atof(argv[1]); // Value of x
    double result = 0.0;

    // Create an array of pipes for communication
    int pipes[n + 1][2];

    for (int i = 0; i <= n; ++i) {
        if (pipe(pipes[i]) == -1) {
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
            double ai = atof(argv[i + 2]);
            double termResult = ai;

            for (int j = n-i; j > 0; j--) {
                termResult *= v;
            }

            // Write the result to the pipe
            write(pipes[i][1], &termResult, sizeof(termResult));
            close(pipes[i][1]);
            exit(0);
        } else {
            // Close the writing end in the parent process
            close(pipes[i][1]);
        }
    }

    // Parent process
    for (int i = 0; i <= n; ++i) {
        // Read the result from the pipe
        double termResult;
        read(pipes[i][0], &termResult, sizeof(termResult));
        close(pipes[i][0]);

        // Accumulate the result
        result += termResult;

        printf("P%d: Term %d = %f\n", i, i, termResult);

        if (i == n) {
            // Last child process prints the total polynomial value
            printf("P%d: Polynomial Value for x=%f: %f\n", i, v, result);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i <= n; ++i) {
        wait(NULL);
    }

    return 0;
}

