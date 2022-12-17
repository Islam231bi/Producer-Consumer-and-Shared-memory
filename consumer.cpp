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

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET "\x1b[0m"

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

    while(1){

        /*Start critical section*/
        buf_data item = b->value[b->next_out];
    	// b->value[b->next_out].price = 0;
    	b->next_out = (b->next_out + 1) % buffer_size;
        /*End of critical section*/

        int cnt = 0;
        // Usage
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
                cout<<"|"<<setw(5)<<setfill(' ')<<"\x1b["<<"34m"<<setw(10);
            else if (color_val[i] == 1){
                cout<<"|"<<setw(5)<<setfill(' ')<<"\x1b["<<"32m"<<setw(10);
            }
            else if (color_val[i] == -1){
                 cout<<"|"<<setw(5)<<setfill(' ')<<"\x1b["<<"31m"<<setw(10);
            }
            cout<<comms_val[i]<<comms_arrow[i];
            
            cout<<"\x1b["<<"0m";

            if (color_avg[i] == 0)
                cout<<"|"<<setw(5)<<setfill(' ')<<"\x1b["<<"34m"<<setw(10);
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