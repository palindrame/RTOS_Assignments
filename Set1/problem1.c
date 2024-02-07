
/*

   Write a complete C program that reads a number  n as command line argument. That is, uses "int argc" and "char *argv[]" to read  n when the program is executed as "./a.out n".The program then creates n child processes P1, P2, ..., Pn such that Pi, 1 ≤  i ≤  n, computes and prints the factorial of i. That is, P1 computes and prints the factorial of 1, P2 computes and prints the factorial of 2, P3 computes and prints the factorial of 3, and so on.

*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

long factorial (int n) {
	if (n == 0 || n == 1) {
		return 1;
	}

	else {
		return n * factorial(n-1);
	}
}


int main (int argc, char *argv[]) {

	if (argc != 2) {
		printf("The correct usage is %s n\n", argv[0]);
		return 1;
	}

	int n = atoi(argv[1]);

	if (n < 1) {
		printf ("Enter n >= 1 \n");
		return 1;
	}

	for (int i = 1; i <= n; i++) {
		
		pid_t pid = fork();
		
		if (pid == -1) {
			perror("fork");
			exit(1);
		}

		else if (pid == 0) {
			printf("Child Process P%d with pid %d has computed the factorial of %d as: %ld\n", i, getpid(), i, factorial(i));
			exit(0);
		}

		else {
			wait(NULL);
		}

	}
	
	return 0;

}




