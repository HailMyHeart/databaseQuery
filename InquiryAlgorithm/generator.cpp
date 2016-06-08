#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "extmem.h"

#define R_BLOCK = 16
#define S_BLOCK = 32
#define TUPLES_PER_BLOCK = 7
#define BUFFER_SIZE = 520
#define BUFFER_MAX_BLOCKS = 8
#define BLOCK_SIZE = 64
using namespace std;
int main()
{
    unsigned int* test = NULL;
    srand((int)time(0));
    unsigned int* block = NULL;
    Buffer buffer;
    initBuffer(BLOCK_SIZE, BLOCK_SIZE, &buffer);
    block = getNewBlockInBuffer(&buffer);
    for(int i = 0; i<R_BLOCK; i++)
    {
        for (int j = 0; j<TUPLES_PER_BLOCK; j++)
        {
            *(block+j*4) = rand()%40+1;
            *(block+j*4+4) = rand()%1000+1;
        }
        *(block+56) = (i+1)*BLOCK_SIZE;
        writeBlockToDisk((unsigned char*)block, (i+1)*BLOCK_SIZE, &buffer);
    }
    for(int i = 0; i<S_BLOCK; i++)
    {
        for (int j = 0; j<TUPLES_PER_BLOCK; j++)
        {
            *(block+j*4) = rand()%40+20;
            *(block+j*4+4) = rand()%1000+1;
        }
        *(block+56) = (i+1)*BLOCK_SIZE;
        writeBlockToDisk((unsigned char*)block, (i+1)*BLOCK_SIZE, &buffer);

    }
    test = (unsigned int *)readBlockFromDisk(0, &buffer);
    return 0;
}
