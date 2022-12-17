#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <cstdio>
#include <string>
#include <string.h>
#include <unistd.h>
#include "test.h"

using namespace std;

int main(int argc , char** argv)
{
	// ftok to generate unique key
	key_t key = ftok("shmfile",65);

	// shmget returns an identifier in shmid
	int shmid = shmget(key,1024,0666|IPC_CREAT);

	// shmat to attach to shared memory
	void *mem_seg = shmat(shmid,(void*)0,0);

	buffer *b = (buffer *) mem_seg;

	while(1){

		b->value[b->next_in].price = 10.21;
        b->value[b->next_in].commodity = 'G';
        b->next_in = (b->next_in + 1) % 4;

		usleep(1000000 * 3);
	}
	
	//detach from shared memory
	shmdt(mem_seg);

	return 0;
}
