#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "helper.h"
#include <chrono>
#include <map>
#include <unistd.h>
#include <cstring>
#include <iomanip>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{

    if(argc != 2){
        printf("usage: [buffer size] ");
        return -1;
    }

    vector<double> comms_val (11,0);
    vector<double> comms_avg (11,0);
    vector<double> comms_prev (11,0);
    vector<double> comms_avg_prev (11,0);
    vector<string> comms_arrow (11," ");
    vector<string> comms_arrow_avg (11," ");

    vector<string> comms = {"ALUMINIUM", "COPEER", "COTTON", "CRUDEOIL", "GOLD", "LEAD", "MENTHAOIL",
    "NATURALGAS", "NICKEL", "SILVER", "ZINC"};

    int buffer_size = stoi(argv[1]);
     
    // ftok to generate unique key
	key_t key = ftok("shmfile",65);

	// shmget returns an identifier in shmid
	int shmid = shmget(key,1024,0666|IPC_CREAT);

	// shmat to attach to shared memory
	void *mem_seg = shmat(shmid,(void*)0,0);

	buffer *b = new (mem_seg) buffer;

    while(1){

        /*Start critical section*/
        buf_data item = b->value[b->next_out];
    	// b->value[b->next_out].price = 0;
    	b->next_out = (b->next_out + 1) % buffer_size;
        /*End of critical section*/

        switch (item.commodity)
        {
        case 'A':
            comms_val[0] = item.price;
            break;
        case 'C':
            comms_val[1] = item.price;
            break;
        case 'T':
            comms_val[2] = item.price;
            break;
        case 'R':
            comms_val[3] = item.price;
            break;
        case 'G':
            comms_val[4] = item.price;
            break;
        case 'L':
            comms_val[5] = item.price;
            break;
        case 'M':
            comms_val[6] = item.price;
            break;
        case 'N':
            comms_val[7] = item.price;
            break;
        case 'K':
            comms_val[8] = item.price;
            break;
        case 'S':
            comms_val[9] = item.price;
            break;
        case 'Z':
            comms_val[10] = item.price;
            break;
        
        default:
            break;
        }

        for(int i = 0 ; i< 11 ; i++){
            if (comms_val[i] > comms_prev[i]){
                //inc
                comms_arrow[i] = "↑";
                comms_prev[i] = comms_val[i];
            }
            else if (comms_val[i] < comms_prev[i]) {
                //dec
                comms_arrow[i] = "↓";
                comms_prev[i] = comms_val[i];
            }
            comms_prev[i] = comms_val[i];
        }

        for(int i = 0 ; i< 11 ; i++){
            if (comms_avg[i] > comms_avg_prev[i]){
                //inc
                comms_arrow_avg[i] = "↑";
                comms_avg_prev[i] = comms_avg[i];
            }
            else if (comms_avg[i] < comms_avg_prev[i]){
                //inc
                comms_arrow_avg[i] = "↓";
                comms_avg_prev[i] = comms_avg[i];
            }
            comms_avg_prev[i] = comms_avg[i];
        }

        // Price formatting
        std::cout << std::fixed;
        std::cout << std::setprecision(2);

        // Clear screen
        printf("\e[1;1H\e[2J");
    
        // Print output
        printf("+--------------------------------------+\n");
        cout<<"|"<<left<<setw(12)<<setfill(' ')<<"Currency";
        cout<<"|"<<left<<setw(12)<<setfill(' ')<<"Price";
        cout<<"|"<<left<<setw(12)<<setfill(' ')<<"AvgPrice    |\n";
        printf("+--------------------------------------+\n");
        for(int i = 0 ; i < 11 ; i++){
            cout<<"|"<<left<<setw(12)<<setfill(' ')<<comms[i];
            cout<<"|"<<left<<setw(12)<<setfill(' ')<<comms_val[i]<<comms_arrow[i];
            cout<<"|"<<left<<setw(12)<<setfill(' ')<<comms_avg[i]<<comms_arrow_avg[i]<<"|\n";
        }
        printf("+--------------------------------------+\n");

        // Delay
		usleep(1000 * 50);    
    }

   //detach from shared memory
	shmdt(mem_seg);
	
	// destroy the shared memory
	shmctl(shmid,IPC_RMID,NULL);

	return 0;
}

/*
//GOLD => G
//SILVER => S
//CRUDEOIL => R
//NATURALGAS => N
//ALUMINIUM => A
//COPPER => C
//NICKEL => K
//LEAD => L
//ZINC => Z
//MENTHAOIL => M
//COTTON => T 
*/