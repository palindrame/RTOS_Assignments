/*A polynomial of degree n (in one variable, with real coefficients) is an expression of the form: anxn + an-1xn-1 + an-2xn-2 + ··· + a2x2 + a1x + a0 where an, an-1, an-2, ··· a2, a1, a0 are real numbers. Example: 3x4 - 2x2 + 5x + 1 is a polynomial of degree 4.

Write a complete C program that reads a polynomial of degree n  through command line arguments. That is, when the program is executed as "./a.out v  an an-1  an-2 ... a2 a1 a0", it uses "int argc" and "char *argv[]" to read an, an-1, an-2, ··· a2, a1, a0 and the value of x (say, v) for which the polynomial is to be evaluated.

The program then creates n+1 child processes P0, P1, P2, ..., Pn such that Pi, 1 ≤  i ≤  n, evaluates the ith term of the polynomial. That is P0 evaluates anvn ,P1 evaluates an-1vn-1 , and so on. Pn, in addition to evaluating the nth term (a0),  also prints the value of the whole polynomial for v. */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <x_value> <coefficient_1> <coefficient_2> ... <coefficient_n>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int n = argc - 3;  // Degree of the polynomial
	double x = atof(argv[1]);

	for (int i = 0; i <= n; i++) {
		pid_t pid = fork();

		if (pid < 0) {
			perror("Fork failed");
			exit(EXIT_FAILURE);
		} else if (pid == 0) { // Child process
			double coefficient = atof(argv[i + 2]);
			double result = coefficient;

			for (int j = n-i; j >0 ; j--) {
				result *= x;
			}

			printf("Term%d(x=%f) = %f\n", i+1, x, result);
			exit(EXIT_SUCCESS);

		}
	}

	// Parent process
	for (int i = 0; i <= n; i++) {
		wait(NULL);
	}

	return 0;
}

