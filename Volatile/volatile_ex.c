/*
 * volatile_ex.c
 * 
 *  
 * Created on: 23-Apr-2017
 * 
 * This example demonstrates the effect of volatile qualifier on a variable.
 * Explanation: How to use this example...
 * the variable counter is declared as a non-volatile auto global variable, under
 * normal compilation condition (without optimization), ie. gcc volatile_ex.c -o volatile_ex -pthread
 * the program works as per the expected logic written i.e. the program will
 * wait on the instruction while(counter == 0); for variable counter value to 
 * become non zero, then loop breaks and the program will print the value and exit.
 * 
 * Now compile the code with gcc -O3 volatile_ex.c -o volatile_ex -pthread, run
 * the code and you will observe that the compiler caches initial value of 
 * variable counter i.e. 0 and the program will now wait on the while loop while(counter == 0);
 * so, whenever you press ctrl+c, it will print the value in Signal Handler 
 * increment the value and whenever you hit ctrl+c, the value incremented from 
 * within signal handler will be printed, the instruction counter++ will have no
 * effect whatsoever.
 *
 * Now, just put volatile qualifier before unsigned int counter, compile the 
 * program with optimization on. gcc -O3 volatile_ex.c -o volatile_ex -pthread
 * Run the program and see the behavior.
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

unsigned int counter = 0;

void *thread0_func(void *arg)
{
	unsigned int i = 0;
	printf("Thread 0\n");
	while(1)
	{
		counter++;

	}

	return NULL;
}

void action_handler(int sig_no)
{
	printf("SigINT Generated: %d\n",counter);
	counter += 1;
}

int main(int argc, char **argv)
{
	pthread_t thread_id[2];

	struct sigaction sa;

	sa.sa_handler = action_handler;

	if(sigaction(SIGINT, &sa, NULL))
		perror("Cannot Install Sig handler");


	if(pthread_create(&thread_id[0], NULL, thread0_func, NULL))
	{
		perror("Error Creating Thread 0");
	}
	else
	{

	}
	while(counter == 0);
	printf("Counter: %d\n", counter);

//	pthread_join(thread_id[0], NULL);
	
	return (0);
}
