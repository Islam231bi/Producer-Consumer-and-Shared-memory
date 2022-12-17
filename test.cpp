#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <cstdio>
#include <string>
#include <unistd.h>
#include "test.h"


using namespace std;

int main(int argc , char** argv)
{
	
    buffer x;
    x.value[0].price = 10;
    x.value[0].commodity = "islam";

    for(int i = 0 ; i < 4 ; i++){
        cout<<x.value[i].price<<"\n";
        cout<<x.value[i].commodity<<"\n";
    }
	return 0;
}
