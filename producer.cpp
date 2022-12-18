#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <random>
#include <chrono>
#include <ctime>
#include <fcntl.h>
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

    // Definitions
    key_t sem_key;
    union semun  
    {
        int val;
        struct semid_ds *buf;
        short array [1];
    } sem_attr;

    // Creating three sets of semaphores each has only 1 semaphore

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
    struct sembuf prod [1];
    prod [0].sem_num = 0;
    prod [0].sem_op = 0;
    prod [0].sem_flg = 0;

    struct timespec tttt;
    struct tm tstruct;
    char data[80];

    while(1){

        // produce 
        double price = distribution(generator);

        time_t now = time(0);
        tstruct = *localtime(&now);
        strftime(data, sizeof(data), "%m/%d/%Y %X", &tstruct);
        clock_gettime( CLOCK_REALTIME, &tttt);
        cout<<"["<<data<<"."<<tttt.tv_nsec/1000000<<"] ";
        cout<<commodity_name<<": "<<"generating a new value "<<price<<"\n";

        // waiting on the full count sem
        prod [0].sem_op = -1;
        if (semop (full_count_sem, prod, 1) == -1) {
	    perror ("semop: full_sem"); exit (1);
        }
    
        tstruct = *localtime(&now);
        strftime(data, sizeof(data), "%m/%d/%Y %X", &tstruct);
        clock_gettime( CLOCK_REALTIME, &tttt);
        cout<<"["<<data<<"."<<tttt.tv_nsec/1000000<<"] ";
        cout<<commodity_name<<": "<<"trying to get mutex on shared buffer...\n";

        // Waiting on the mutex
        prod [0].sem_op = -1;
        if (semop (mutex, prod, 1) == -1) {
	    perror ("semop: mutex_sem"); exit (1);
        }

        /* Start of critical section*/
        tstruct = *localtime(&now);
        strftime(data, sizeof(data), "%m/%d/%Y %X", &tstruct);
        clock_gettime( CLOCK_REALTIME, &tttt);
        cout<<"["<<data<<"."<<tttt.tv_nsec/1000000<<"] ";
        cout<<commodity_name<<": "<<"placing "<<price<<" on shared buffer\n";

		b->value[b->next_in].price = price;
        b->value[b->next_in].commodity = mp[commodity_name];
        b->next_in = (b->next_in + 1) % buffer_size;
        /*End of critical section*/

        // Signaling mutex
        prod [0].sem_op = 1;
        if (semop (mutex, prod, 1) == -1) {
	    perror ("semop: mutex_sem"); exit (1);
        }

        // Signaling empty count sem
        prod [0].sem_op = 1;
        if (semop (empty_count_sem, prod, 1) == -1) {
	    perror ("semop: empty_sem"); exit (1);
        }

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
