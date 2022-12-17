#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "helper.h"
#include <chrono>
#include <map>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv)
{

    if(argc != 2){
        printf("usage: [buffer size] ");
        return -1;
    }

    map<char,string> mp;
    mp['G'] = "GOLD";
    mp['S'] = "SILVER";
    mp['R'] = "CRUDEOIL";
    mp['N'] = "NATURALGAS";
    mp['A'] = "ALUMINIUM";
    mp['C'] = "COPPER";
    mp['K'] = "NICKEL";
    mp['L'] = "LEAD";
    mp['Z'] = "ZINC";
    mp['M'] = "MENTHAOIL";
    mp['T'] = "COTTON";

    int buffer_size = stoi(argv[1]);
     
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


/*
GOLD => G
SILVER => S
CRUDEOIL => R
NATURALGAS => N
ALUMINIUM => A
COPPER => C
NICKEL => K
LEAD => L
ZINC => Z
MENTHAOIL => M
COTTON => T 
*/