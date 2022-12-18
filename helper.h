#include <iostream>
#include <memory>

using namespace std;

// 16 bytes
typedef struct buf_data {
    char commodity= 'F';
    double price = 0;
} buf_data;

// 8 + size * 16 
typedef struct buffer {
    buf_data value[100];
    int next_in = 0, next_out = 0;
} buffer;