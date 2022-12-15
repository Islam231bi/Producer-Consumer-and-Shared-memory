#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string>
#include <random>
#include <chrono>
#include<unistd.h>

using namespace std;


int main(int argc, char** argv)
{
    string commodity_name = argv[1];
    double commodity_price_mean = stod(argv[2]);
    double commodity_dev = stod(argv[3]);
    int sleep_val = stoi(argv[4]);
    int buffer_size = stoi(argv[5]);

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(commodity_price_mean,commodity_dev);

    while(1){
        double price = distribution(generator);

        /* Start of critical section*/
        std::cout<<"price: "<<price<<"\n";
        
        /*End of critical section*/

        // delay
        usleep(1000 * sleep_val);
    }
    
	return 0;
}
