#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <random>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <map>
#include <unistd.h>
#include "helper.h"

using namespace std;


int main(int argc, char** argv)
{
    if(argc != 6){
        printf("usage: [commodity name] [price_mean] [standard deviation] [delay in millis] [buffer size] ");
        return -1;
    }

    string commodity_name = argv[1];
    double commodity_price_mean = stod(argv[2]);
    double commodity_dev = stod(argv[3]);
    int sleep_val = stoi(argv[4]);
    int buffer_size = stoi(argv[5]);

    map<string,char> mp;
    mp["GOLD"] = 'G';
    mp["SILVER"] = 'S';
    mp["CRUDEOIL"] = 'R';
    mp["NATURALGAS"] = 'N';
    mp["ALUMINIUM"] = 'A';
    mp["COPPER"] = 'C';
    mp["NICKEL"] = 'K';
    mp["LEAD"] = 'L';
    mp["ZINC"] = 'Z';
    mp["MENTHAOIL"] = 'M';
    mp["COTTON"] = 'T';

    // Price generator
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(commodity_price_mean,commodity_dev);

    // ftok to generate unique key
	key_t key = ftok("shmfile",65);

	// shmget returns an identifier in shmid
	int shmid = shmget(key,1024,0666|IPC_CREAT);

	// shmat to attach to shared memory
	void *mem_seg = shmat(shmid,(void*)0,0);

	buffer *b = (buffer *) mem_seg;

    struct timespec tttt;
    struct tm tstruct;
    char data[80];

    while(1){
        
        double price = distribution(generator);
        
        time_t now = time(0);
        tstruct = *localtime(&now);
        strftime(data, sizeof(data), "%m/%d/%Y %X", &tstruct);
        clock_gettime( CLOCK_REALTIME, &tttt);
        cout<<"["<<data<<"."<<tttt.tv_nsec/1000000<<"] ";
        cout<<commodity_name<<": "<<"generating a new value "<<price<<"\n";
        

        /* Start of critical section*/
		b->value[b->next_in].price = price;
        b->value[b->next_in].commodity = mp[commodity_name];
        b->next_in = (b->next_in + 1) % buffer_size;
        /*End of critical section*/

        tstruct = *localtime(&now);
        strftime(data, sizeof(data), "%m/%d/%Y %X", &tstruct);
        clock_gettime( CLOCK_REALTIME, &tttt);
        cout<<"["<<data<<"."<<tttt.tv_nsec/1000000<<"] ";
        cout<<commodity_name<<": "<<"sleeping for "<<sleep_val<<" ms\n";
        
        // Delay
        usleep(1000 * sleep_val);
    }

    //detach from shared memory
	shmdt(mem_seg);
    
	return 0;
}
