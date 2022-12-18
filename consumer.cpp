#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <stdio.h>
#include "helper.h"
#include <chrono>
#include <map>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
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
    vector<int> color_val (11, 0);
    vector<int> color_avg (11, 0);

    vector<double> vals[11];


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

    // Definitions
    key_t sem_key;
    union semun  
    {
        int val;
        struct semid_ds *buf;
        short array [1];
    } sem_attr;
    int mutex, full_count_sem, empty_count_sem;

    if ((sem_key = ftok (MUTEX_KEY, 65)) == -1) {
        perror ("ftok"); exit (1);
    }
    if ((mutex = semget (sem_key, 1, 0660 | IPC_CREAT)) == -1) {
        perror ("semget"); exit (1);
    }

    sem_attr.val = 1;
    if (semctl (mutex, 0, SETVAL, sem_attr) == -1) {
        perror ("semctl SETVAL"); exit (1);
    }
    
    if ((sem_key = ftok (SEM_FULL_KEY, 65)) == -1) {
        perror ("ftok"); exit (1);
    }
    if ((full_count_sem = semget (sem_key, 1, 0660 | IPC_CREAT)) == -1) {
        perror ("semget"); exit (1);
    }
   
    sem_attr.val = buffer_size; 
    if (semctl (full_count_sem, 0, SETVAL, sem_attr) == -1) {
        perror (" semctl SETVAL "); exit (1);
    }


    if ((sem_key = ftok (SEM_EMPTY_KEY, 65)) == -1) {
        perror ("ftok"); exit (1);
    }
    if ((empty_count_sem = semget (sem_key, 1, 0660 | IPC_CREAT)) == -1) {
        perror ("semget"); exit (1);
    }
    
    sem_attr.val = 0;
    if (semctl (empty_count_sem, 0, SETVAL, sem_attr) == -1) {
        perror (" semctl SETVAL "); exit (1);
    }

    // Semaphore stuff
    struct sembuf cons [1];
    cons [0].sem_num = 0;
    cons [0].sem_op = 0;
    cons [0].sem_flg = 0;

    while(1){

        // Wating on empty_sem 
        cons [0].sem_op = -1;
        if (semop (empty_count_sem, cons, 1) == -1) {
	    perror ("semop: empty_sem"); exit (1);
        }

        // Waiting on the mutex
        cons [0].sem_op = -1;
        if (semop (mutex, cons, 1) == -1) {
	    perror ("semop: mutex_sem"); exit (1);
        }

        /*Start critical section*/
        buf_data item = b->value[b->next_out];
    	b->value[b->next_out].price = 0;
    	b->next_out = (b->next_out + 1) % buffer_size;
        /*End of critical section*/

        // Signaling mutex
        cons [0].sem_op = 1;
        if (semop (mutex, cons, 1) == -1) {
	    perror ("semop: mutex_sem"); exit (1);
        }

        // Signaling full_sem
        cons [0].sem_op = 1;
        if (semop (full_count_sem, cons, 1) == -1) {
	    perror ("semop: full_sem"); exit (1);
        }

        // Usage
        int cnt = 0;
        switch (item.commodity)
        {
        case 'A':
            comms_val[0] = item.price;
            vals[0].push_back(item.price);
            cout<<"dasd\n";
            comms_avg[0] = 0;
            cnt = 0; 
            for(int i = vals[0].size() - 1, x = 0 ; i >= 0 && x < 5 ; i--, x++){
                comms_avg[0]+=vals[0][i];
                cnt++;
            }
            comms_avg[0]/=cnt;
            
            break;
        case 'C':
            comms_val[1] = item.price;
            vals[1].push_back(item.price);
            comms_avg[1] = 0;
            cnt = 0;  // 7 --> 
            for(int i = vals[1].size() - 1, x = 0 ; i >= 0 && x < 5 ; i--, x++){
                comms_avg[1]+=vals[1][i];
                cnt++;
            }
            comms_avg[1]/=cnt;
            break;
        case 'T':
            comms_val[2] = item.price;
            vals[2].push_back(item.price);
            comms_avg[2] = 0;
            cnt = 0;  // 7 --> 
            for(int i = vals[2].size() - 1, x = 0 ; i >= 0 && x < 5 ; i--, x++){
                comms_avg[2]+=vals[2][i];
                cnt++;
            }
            comms_avg[2]/=cnt;
            break;
        case 'R':
            comms_val[3] = item.price;
            vals[3].push_back(item.price);
            comms_avg[3] = 0;
            cnt = 0;  // 7 --> 
            for(int i = vals[3].size() - 1, x = 0 ; i >= 0 && x < 5 ; i--, x++){
                comms_avg[3]+=vals[3][i];
                cnt++;
            }
            comms_avg[3]/=cnt;
            break;
        case 'G':
            comms_val[4] = item.price;
            vals[4].push_back(item.price);
            comms_avg[4] = 0;
            cnt = 0;  // 7 --> 
            for(int i = vals[4].size() - 1, x = 0 ; i >= 0 && x < 5 ; i--, x++){
                comms_avg[4]+=vals[4][i];
                cnt++;
            }
            comms_avg[4]/=cnt;
            break;
        case 'L':
            comms_val[5] = item.price;
            vals[5].push_back(item.price);
            comms_avg[5] = 0;
            cnt = 0;  // 7 --> 
            for(int i = vals[5].size() - 1, x = 0 ; i >= 0 && x < 5 ; i--, x++){
                comms_avg[5]+=vals[5][i];
                cnt++;
            }
            comms_avg[5]/=cnt;
            break;
        case 'M':
            comms_val[6] = item.price;
            vals[6].push_back(item.price);
            comms_avg[6] = 0;
            cnt = 0;  // 7 --> 
            for(int i = vals[6].size() - 1, x = 0 ; i >= 0 && x < 5 ; i--, x++){
                comms_avg[6]+=vals[6][i];
                cnt++;
            }
            comms_avg[6]/=cnt;
            break;
        case 'N':
            comms_val[7] = item.price;
            vals[7].push_back(item.price);
            comms_avg[7] = 0;
            cnt = 0;  // 7 --> 
            for(int i = vals[7].size() - 1, x = 0 ; i >= 0 && x < 5 ; i--, x++){
                comms_avg[7]+=vals[7][i];
                cnt++;
            }
            comms_avg[7]/=cnt;
            break;
        case 'K':
            comms_val[8] = item.price;
            vals[8].push_back(item.price);
            comms_avg[8] = 0;
            cnt = 0;  // 7 --> 
            for(int i = vals[8].size() - 1, x = 0 ; i >= 0 && x < 5 ; i--, x++){
                comms_avg[8]+=vals[8][i];
                cnt++;
            }
            comms_avg[8]/=cnt;
            break;
        case 'S':
            comms_val[9] = item.price;
            vals[9].push_back(item.price);
            comms_avg[9] = 0;
            cnt = 0;  // 7 --> 
            for(int i = vals[9].size() - 1, x = 0 ; i >= 0 && x < 5 ; i--, x++){
                comms_avg[9]+=vals[9][i];
                cnt++;
            }
            comms_avg[9]/=cnt;
            break;
        case 'Z':
            comms_val[10] = item.price;
            vals[10].push_back(item.price);
            comms_avg[10] = 0;
            cnt = 0;  // 7 --> 
            for(int i = vals[10].size() - 1, x = 0 ; i >= 0 && x < 5 ; i--, x++){
                comms_avg[10]+=vals[10][i];
                cnt++;
            }
            comms_avg[10]/=cnt;
            break;
        
        default:
            break;
        }

        for(int i = 0 ; i< 11 ; i++){
            if (comms_val[i] > comms_prev[i]){
                //inc
                comms_arrow[i] = "↑";
                comms_prev[i] = comms_val[i];
                color_val[i] = 1; // green

            }
            else if (comms_val[i] < comms_prev[i]) {
                //dec
                comms_arrow[i] = "↓";
                comms_prev[i] = comms_val[i];
                color_val[i] = -1; //red
            }
            else {
                comms_prev[i] = comms_val[i];
                color_val[i] = 0; //blue
                comms_arrow[i] = " ";
            }
            
        }

        for(int i = 0 ; i< 11 ; i++){
            if (comms_avg[i] > comms_avg_prev[i]){
                //inc
                comms_arrow_avg[i] = "↑";
                comms_avg_prev[i] = comms_avg[i];
                color_avg[i] = 1;
            }
            else if (comms_avg[i] < comms_avg_prev[i]){
                //inc
                comms_arrow_avg[i] = "↓";
                comms_avg_prev[i] = comms_avg[i];
                color_avg[i] = -1;
            }
            else{
                comms_avg_prev[i] = comms_avg[i];
                color_avg[i] = 0;
                comms_arrow_avg[i] = " ";
            }
            
        }

        // Price formatting
        std::cout << std::fixed;
        std::cout << std::setprecision(2);

        // Clear screen
        printf("\e[1;1H\e[2J");
    
        // Print output
        printf("+-----------------------------------------------+\n");
        cout<<"|"<<setw(13)<<setfill(' ')<<"Currency"<<setw(5);
        cout<<"|"<<setw(2)<<setfill(' ')<<"Price"<<setw(10);
        cout<<"|"<<setw(2)<<setfill(' ')<<"AvgPrice      |\n";
        printf("+-----------------------------------------------+\n");
        for(int i = 0 ; i < 11 ; i++){
            cout<<"|"<<setw(13)<<setfill(' ')<<comms[i]<<setw(5);
            if (color_val[i] == 0)
                cout<<"|"<<setw(5)<<setfill(' ')<<"\x1b["<<"36m"<<setw(10);
            else if (color_val[i] == 1){
                cout<<"|"<<setw(5)<<setfill(' ')<<"\x1b["<<"32m"<<setw(10);
            }
            else if (color_val[i] == -1){
                 cout<<"|"<<setw(5)<<setfill(' ')<<"\x1b["<<"31m"<<setw(10);
            }
            cout<<comms_val[i]<<comms_arrow[i];
            
            cout<<"\x1b["<<"0m";

            if (color_avg[i] == 0)
                cout<<"|"<<setw(5)<<setfill(' ')<<"\x1b["<<"36m"<<setw(10);
            else if (color_avg[i] == 1){
                cout<<"|"<<setw(5)<<setfill(' ')<<"\x1b["<<"32m"<<setw(10);
            }
            else if (color_avg[i] == -1){
                 cout<<"|"<<setw(5)<<setfill(' ')<<"\x1b["<<"31m"<<setw(10);
            }
            cout<<comms_avg[i]<<comms_arrow_avg[i];
            cout<<"\x1b["<<"0m";
            cout<<"|\n";
        }
        printf("+-----------------------------------------------+\n");

        // Delay
		usleep(1000 * 200);    
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