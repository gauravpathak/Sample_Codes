/*
 * volatile_ex.c
 *
 *  Created on: 23-Apr-2017
 *      Author: gaurav
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
//	counter += 1;
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

	return (0);
}
