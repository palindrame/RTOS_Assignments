/*
 Let there be a (parent) process which goes on putting a random (integer) number  in a shared variable, say n,  in a regular interval. There is a child process that goes on putting the factorial of the integer (that it finds in n) back into the shared variable (n), in regular intervals. The parent process prints the factorial that it receives from the child process. Device your own mechanism(s) so that the child process "knows" that a new number has been put in n  by the parent process and the parent process "knows" that a the factorial of a number has been put in n  by the child  process,

Write a complete C program to implement the above. The program should make arrangements for releasing shared memory that has been created during execution.

The processes must print meaningful output so that the user understands what is happening.
*/



#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/shm.h>
#include <signal.h>

//typedef void (*signalhandler_t)(int);
void controlSHM(int);
void calculateFactorial(int, int*);
int shmid;
pid_t pid;
int* sharedNum;

int main() {

  //signalhandler_t shandler;

  shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0777);

  if(shmid == -1) {
    perror("Error in shmget()!\n");
    exit(1);
  }

  pid = fork();
  if(pid == -1){
    perror("Error in fork()!\n");
    exit(1);
  }

  if(pid == 0) {
    signal(SIGUSR1, controlSHM);
  }

  else {
    signal(SIGUSR2, controlSHM);
  }
  
  wait(NULL);
  if (shmdt(sharedNum) == -1) {
    perror("Error in shmdt()!\n");
    exit(1);
  }

  if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    perror("Error in shmctl()!\n");
    exit(1);
  }
  return 0;
}

void controlSHM (int signum) {

  sharedNum = shmat(shmid, NULL, 0);
  *sharedNum = 0;

  if (sharedNum == (int*)-1) {
    perror("Error in shmat()!\n");
    exit(1);
  }

  if (signum == SIGUSR1) {
    printf("(Child)The value in the shared memory segment currently is: %d\n", *sharedNum);
    int factorial;
    calculateFactorial(*sharedNum, &factorial);
    *sharedNum = factorial; 
    kill(getppid(), SIGUSR2);
  }

  else if (signum == SIGUSR2) {
    printf("(Parent)The value in the shared memory segment currently is: %d\n", *sharedNum);
    int randomNum = rand() % 10 + 1;
    *sharedNum = randomNum;
    kill(pid, SIGUSR1);
  }

}

void calculateFactorial(int num, int* result) {
  *result = 1;
  for(int i = 1; i <= num; i++) {
    *result *= i;
  }
}












