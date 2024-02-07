/*
 * Write a complete C program that reads 2 numbers  n1 and n2  (n1≤n2) as command line arguments. That is, uses "int argc" and "char *argv[]" to read  n1 and n2 when the program is executed as "./a.out n1 n2". The program then creates n2 - n1 + 1 child processes Pn1, Pn1+1, ..., Pn2 such that Pi, n1 ≤  i ≤  n2, computes and prints the factorial of i. That is Pn1 computes and prints the factorial of n1, Pn1+1 computes and prints the factorial of n1+1, and so on!
Additionally ensure that process Pi uses the value computed by Pi-1. That is, while computing i! (factorial of i) Pi uses  (i-1)! computed by Pi-1.
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
	
	if (argc != 3) {
		printf("Correct usage is: %s <n1> <n2>.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int n1 = atoi(argv[1]);
	int n2 = atoi(argv[2]);

	if (n1 > n2) {
		printf("The value of n1 <= n2.\n");
		exit(EXIT_FAILURE);
	}

	int fact = 1; pid_t pid;
	for(int i = 1; i <= n1; i++) {
		fact *= i;
	}

	for(int j = n1; j <= n2; j++) {
		pid = fork();
		if (pid < 0) {
			printf("Error in creating child process.\n");
		}

		if (pid == 0) {
			exit(fact);
		}

		else {
			int status;
			wait(&status);
			printf("The factorial of %d is: %d.\n", j, WEXITSTATUS(status)*j);
		}
	}

	return 0;
}
