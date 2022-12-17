#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <queue>
#include "test.h"

using namespace std;


int main()
{
	// ftok to generate unique key
	key_t key = ftok("shmfile",65);

	// shmget returns an identifier in shmid
	int shmid = shmget(key,1024,0666|IPC_CREAT);

	// shmat to attach to shared memory
	void *mem_seg = shmat(shmid,(void*)0,0);

	buffer *b = new (mem_seg) buffer;

	while(1){
		buf_data item = b->value[b->next_out];
        // item.price = b->value[b->next_out].price;
        // item.commodity = b->value[b->next_out].commodity;
    	b->value[b->next_out].price = 0;
    	b->next_out = (b->next_out + 1) % 4;

        cout<<"===============\n";
        cout<<item.commodity<<"\n";
		cout<<item.price<<"\n";
        cout<<"===============\n";

		usleep(1000 * 3000);   
	}
	
	//detach from shared memory
	shmdt(mem_seg);
	
	// destroy the shared memory
	shmctl(shmid,IPC_RMID,NULL);
	
	return 0;
}
