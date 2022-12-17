#include <iostream>

using namespace std;


typedef struct buf_data {
    char commodity= 'F';
    double price = 0.0;
} buf_data;

typedef struct buffer {
    buf_data value[4];
    int next_in = 0, next_out = 0;
} buffer;


