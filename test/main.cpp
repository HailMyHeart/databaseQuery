#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <vector>
#include <algorithm>
#include "extmem.h"
#define BYTES_PER_TUPLE 8
#define R_BLOCK  16
#define S_BLOCK  32
#define TUPLES_PER_BLOCK  7
#define BUFFER_SIZE  520
#define BUFFER_MAX_BLOCKS  8
#define BLOCK_SIZE  64
#define R_ADDRESS 0
#define S_ADDRESS R_BLOCK*BLOCK_SIZE
typedef struct sort_Struct
{
    unsigned int a;
    unsigned int b;
}sortStruct;
bool compare(sortStruct A,sortStruct B)
{
      return A.a>B.a;
}
using namespace std;
int main()
{
    unsigned int selectionAddress = 0;
    unsigned int projectionAddress = 0;
    unsigned int hash_R[7];
    unsigned int hash_S[7];
    unsigned int hash_join = 0;
    unsigned int nest_join = 0;
    unsigned int sorted_R = 0;
    unsigned int sorted_S = 0;
    unsigned int sorted_base_union = 0;
    unsigned int nextDiskAddress = 0;
    unsigned int nextBlockToRead = 0;
    unsigned int* test = NULL;
    srand(0);
    unsigned int* block = NULL;
    Buffer buffer;
    initBuffer(BUFFER_SIZE, BLOCK_SIZE, &buffer);


    for(int i = 0; i<R_BLOCK; i++)
    {
        block = (unsigned int*)getNewBlockInBuffer(&buffer);
        for (int j = 0; j<TUPLES_PER_BLOCK; j++)
        {
            *(block+j*2) = (unsigned int)(rand()%40+1);     //R.A ¡Ê[1,40]
            *(block+j*2+1) =(unsigned int)(rand()%1000+1);  //R.S¡Ê[1,1000]
            cout<<*(block+j*2)<<' '<<*(block+j*2+1)<<' ';
        }
        *(block+TUPLES_PER_BLOCK*2) = 14;           //the count of integer one block holds
        *(block+TUPLES_PER_BLOCK*2+1) = (i == R_BLOCK-1?0:(i+1)*BLOCK_SIZE);    //next address
        cout<<*(block+TUPLES_PER_BLOCK*2+1)<<endl;

        writeBlockToDisk((unsigned char*)block, i*BLOCK_SIZE, &buffer);
        // read the block write to disk right now to test
        test = (unsigned int*)readBlockFromDisk(i*BLOCK_SIZE, &buffer);
        cout<<" "<<"12th integer:"<<test[12]<<",next address:"<<test[15]<<endl;    //the 12th integer and the next address
        freeBlockInBuffer((unsigned char*)test, &buffer);
    }
    nextDiskAddress = S_ADDRESS;
    cout<<buffer.numAllBlk<<" "<<buffer.numFreeBlk<<endl;
    for(int i = 0; i<S_BLOCK; i++)
    {
        block = (unsigned int*)getNewBlockInBuffer(&buffer);
        for (int j = 0; j<TUPLES_PER_BLOCK; j++)
        {
            *(block+j*2) = (unsigned int)(rand()%40+20);
            *(block+j*2+1) = (unsigned int)(rand()%1000+1);
        }
        *(block+TUPLES_PER_BLOCK*2) = 14;
        *(block+TUPLES_PER_BLOCK*2+1) = (i == S_BLOCK-1?0:(i+17)*BLOCK_SIZE);
        nextDiskAddress += BLOCK_SIZE;
        writeBlockToDisk((unsigned char*)block, (i+16)*BLOCK_SIZE, &buffer);
        cout<<*(block+TUPLES_PER_BLOCK*2+1)<<endl;
                // read the block write to disk right now to test
        test = (unsigned int*)readBlockFromDisk((i+16)*BLOCK_SIZE, &buffer);
        cout<<" "<<"12th integer:"<<test[12]<<",next address:"<<test[15]<<endl;    //the 12th integer and the next address
        freeBlockInBuffer((unsigned char*)test, &buffer);

    }
   test = (unsigned int *)readBlockFromDisk(0, &buffer);
    cout<<*(test+0)<<" "<<*(test+TUPLES_PER_BLOCK*2)<<endl;
    freeBlockInBuffer((unsigned char*)block, &buffer);
    cout<<buffer.numAllBlk<<" "<<buffer.numFreeBlk<<endl;
//
//
//
//    dropBlockOnDisk(384);
//    block = (unsigned int*)getNewBlockInBuffer(&buffer);
//    for (int i = 0 ; i<TUPLES_PER_BLOCK; i++)
//    {
//        *(block+i*2) = (unsigned int)(rand()%40+1);
//        *(block+i*2+1) = (unsigned int)(rand()%1000+1);
//    }
//
//    *(block+TUPLES_PER_BLOCK*2) = 14;           //the count of integer one block holds
//    *(block+TUPLES_PER_BLOCK*2+1) = 448;    //next address
//    writeBlockToDisk((unsigned char*)block, 384, &buffer);
//        // read the block write to disk right now to test
//    test = (unsigned int*)readBlockFromDisk(3072, &buffer);
//    cout<<" "<<"12th integer:"<<test[2]<<",next address:"<<test[15]<<endl;    //the 12th integer and the next address
//    freeBlockInBuffer((unsigned char*)test, &buffer);





//
//
//
    //select
//    nextDiskAddress = 3072;
//    selectionAddress = nextDiskAddress;
//    unsigned int* outputBuffer = (unsigned int*)getNewBlockInBuffer(&buffer);
//    block = (unsigned int*)readBlockFromDisk(R_ADDRESS, &buffer);
//
//    nextBlockToRead = R_ADDRESS;
//    int outBufferInts = 0;
//    while(block[TUPLES_PER_BLOCK*2+1] != 0)
//    {
//        for (int j = 0; j<TUPLES_PER_BLOCK; j++)
//        {
//            if(*(block+j*2) == 40)
//            {
//                outputBuffer[outBufferInts] = block[j*2];
//                outputBuffer[outBufferInts+1] = block[j*2+1];
//                outBufferInts += 2;
//                if (outBufferInts == 14)
//                {
//                    outBufferInts = 0;
//                    outputBuffer[TUPLES_PER_BLOCK*2] = 14;
//                    outputBuffer[TUPLES_PER_BLOCK*2+1] = nextDiskAddress+BLOCK_SIZE;
//                    writeBlockToDisk((unsigned char*)outputBuffer, nextDiskAddress, &buffer);
//                    nextDiskAddress += BLOCK_SIZE;
//                    outputBuffer = (unsigned int*)getNewBlockInBuffer(&buffer);
//                }
//
//            }
//        }
//        nextBlockToRead = block[TUPLES_PER_BLOCK*2+1];
//        freeBlockInBuffer((unsigned char*)block, &buffer);
//        block = (unsigned int*)readBlockFromDisk(nextBlockToRead, &buffer);
//
//    }
//    for (int j = 0; j<TUPLES_PER_BLOCK; j++)
//    {
//        if(*(block+j*2) == 40)
//        {
//            outputBuffer[outBufferInts] = block[j*2];
//            outputBuffer[outBufferInts+1] = block[j*2+1];
//            outBufferInts += 2;
//            if (outBufferInts == 14)
//            {
//                outBufferInts = 0;
//                outputBuffer[TUPLES_PER_BLOCK*2] = 14;
//                outputBuffer[TUPLES_PER_BLOCK*2+1] = nextDiskAddress+BLOCK_SIZE;
//                writeBlockToDisk((unsigned char*)outputBuffer, nextDiskAddress, &buffer);
//                nextDiskAddress += BLOCK_SIZE;
//                outputBuffer = (unsigned int*)getNewBlockInBuffer(&buffer);
//            }
//
//        }
//    }
//    freeBlockInBuffer((unsigned char*)block, &buffer);
//    block = (unsigned int*)readBlockFromDisk(S_ADDRESS, &buffer);
//    while(block[TUPLES_PER_BLOCK*2+1] != 0)
//    {
//        for (int j = 0; j<TUPLES_PER_BLOCK; j++)
//        {
//
//            if(*(block+j*2) == 60)
//            {
//                outputBuffer[outBufferInts] = block[j*2];
//                outputBuffer[outBufferInts+1] = block[j*2+1];
//                outBufferInts += 2;
//                if (outBufferInts == 14)
//                {
//                    outBufferInts = 0;
//                    outputBuffer[TUPLES_PER_BLOCK*2] = 14;
//                    outputBuffer[TUPLES_PER_BLOCK*2+1] = nextDiskAddress+BLOCK_SIZE;
//                    writeBlockToDisk((unsigned char*)outputBuffer, nextDiskAddress, &buffer);
//                    nextDiskAddress += BLOCK_SIZE;
//                    outputBuffer = (unsigned int*)getNewBlockInBuffer(&buffer);
//                }
//
//            }
//        }
//        nextBlockToRead = block[TUPLES_PER_BLOCK*2+1];
//        freeBlockInBuffer((unsigned char*)block, &buffer);
//        block = (unsigned int*)readBlockFromDisk(nextBlockToRead, &buffer);
//    }
//    for (int j = 0; j<TUPLES_PER_BLOCK; j++)
//    {
//        if(*(block+j*2) == 60)
//        {
//            outputBuffer[outBufferInts] = block[j*2];
//            outputBuffer[outBufferInts+1] = block[j*2+1];
//            outBufferInts += 2;
//            if (outBufferInts == 14 && j != TUPLES_PER_BLOCK-1)
//            {
//                outBufferInts = 0;
//                outputBuffer[TUPLES_PER_BLOCK*2] = 14;
//                outputBuffer[TUPLES_PER_BLOCK*2+1] = nextDiskAddress+BLOCK_SIZE;
//                writeBlockToDisk((unsigned char*)outputBuffer, nextDiskAddress, &buffer);
//                nextDiskAddress += BLOCK_SIZE;
//                outputBuffer = (unsigned int*)getNewBlockInBuffer(&buffer);
//            }
//
//        }
//        if (j == TUPLES_PER_BLOCK-1)
//        {
//
//            outputBuffer[TUPLES_PER_BLOCK*2] = outBufferInts;
//            outBufferInts = 0;
//            outputBuffer[TUPLES_PER_BLOCK*2+1] = 0;
//            writeBlockToDisk((unsigned char*)outputBuffer, nextDiskAddress, &buffer);
//            nextDiskAddress += BLOCK_SIZE;
//            outputBuffer  =  (unsigned int*)getNewBlockInBuffer(&buffer);
//        }
//    }
//    freeBlockInBuffer((unsigned char*)block ,&buffer);
////projection :A of R
//    projectionAddress = nextDiskAddress;
//    block = (unsigned int*)readBlockFromDisk(R_ADDRESS, &buffer);
//    while(block[TUPLES_PER_BLOCK*2+1] != 0)
//    {
//        for (int j = 0; j<TUPLES_PER_BLOCK; j++)
//        {
//
//            outputBuffer[outBufferInts] = block[j*2];
//            outBufferInts ++;
//            if (outBufferInts == 14)
//            {
//                outBufferInts = 0;
//                outputBuffer[TUPLES_PER_BLOCK*2] = 14;
//                outputBuffer[TUPLES_PER_BLOCK*2+1] = nextDiskAddress+BLOCK_SIZE;
//                writeBlockToDisk((unsigned char*)outputBuffer, nextDiskAddress, &buffer);
//                nextDiskAddress += BLOCK_SIZE;
//                outputBuffer = (unsigned int*)getNewBlockInBuffer(&buffer);
//            }
//        }
//        nextBlockToRead = block[TUPLES_PER_BLOCK*2+1];
//        freeBlockInBuffer((unsigned char*)block, &buffer);
//        block = (unsigned int*)readBlockFromDisk(nextBlockToRead, &buffer);
//
//
//    }
//    for (int j = 0; j<TUPLES_PER_BLOCK; j++)
//    {
//        outputBuffer[outBufferInts] = block[j*2];
//        outBufferInts ++;
//        if (outBufferInts == 14 && j != TUPLES_PER_BLOCK-1)
//        {
//            outBufferInts = 0;
//            outputBuffer[TUPLES_PER_BLOCK*2] = 14;
//            outputBuffer[TUPLES_PER_BLOCK*2+1] = nextDiskAddress+BLOCK_SIZE;
//            writeBlockToDisk((unsigned char*)outputBuffer, nextDiskAddress, &buffer);
//            nextDiskAddress += BLOCK_SIZE;
//            outputBuffer = (unsigned int*)getNewBlockInBuffer(&buffer);
//        }
//
//        if (j == TUPLES_PER_BLOCK-1)
//        {
//
//            outputBuffer[TUPLES_PER_BLOCK*2] = outBufferInts;
//            outBufferInts = 0;
//            outputBuffer[TUPLES_PER_BLOCK*2+1] = 0;
//            writeBlockToDisk((unsigned char*)outputBuffer, nextDiskAddress, &buffer);
//            nextDiskAddress += BLOCK_SIZE;
//            outputBuffer  = (unsigned int*)getNewBlockInBuffer(&buffer);
//        }
//    }
//    freeBlockInBuffer((unsigned char*)block ,&buffer);
//
//
//
//
//
//
//
//    //nest_loop_join
//    int nest_blocks = 0;
//    unsigned int *blockList[8-2];
//    nest_join = nextDiskAddress;
//    for (int i = 0; i<R_BLOCK; i++)
//    {
//        blockList[i%6] = (unsigned int*)readBlockFromDisk(i*BLOCK_SIZE, &buffer);
//        if (i == R_BLOCK-1 || i%6 == 5)
//        {
//            for (int j = 0; j<S_BLOCK; j++)
//            {
//                block = (unsigned int*)readBlockFromDisk(S_ADDRESS+j*BLOCK_SIZE, &buffer);
//                for (int k = 0; k<TUPLES_PER_BLOCK; k++)
//                {
//                    for (int x = 0; x<=i%6; x++)
//                    {
//                        for (int y = 0 ; y<TUPLES_PER_BLOCK; y++)
//                        {
//                            if (blockList[x][y*2] == block[k*2])
//                            {
//
//                                outputBuffer[0] = blockList[x][y*2];
//                                outputBuffer[1] = blockList[x][y*2+1];
//                                outputBuffer[2] = block[k*2];
//                                outputBuffer[3] = block[k*2+1];
//                                outputBuffer[14] = 4;
//                                outputBuffer[15] = nextDiskAddress+BLOCK_SIZE;
//                                writeBlockToDisk((unsigned char*)outputBuffer, nextDiskAddress, &buffer );
//                                nextDiskAddress += BLOCK_SIZE;
//                                outputBuffer = (unsigned int*)getNewBlockInBuffer(&buffer);
//                                nest_blocks++;
//
//                            }
//                        }
//                    }
//                }
//                freeBlockInBuffer((unsigned char*)block, &buffer);
//
//            }
//            for (int j = 0; j<=i%6; j++)
//            {
//                freeBlockInBuffer((unsigned char*)blockList[j], &buffer);
//            }
//        }
//    }
//    freeBlockInBuffer((unsigned char*)outputBuffer, &buffer);
//
//
//
//    //two phase sort
//    vector<unsigned int> address;
//    address.push_back(nextDiskAddress);
//    unsigned int* sortList[8];
//
//    vector<sortStruct> sortVector;
//    for (int i = 0 ; i<R_BLOCK/8; i++)
//    {
//        for (int j = i*8; j<i*8+8; j++)
//        {
//            sortList[j-i*8] = (unsigned int*)readBlockFromDisk(R_ADDRESS+j*BLOCK_SIZE, &buffer);
//        }
//        for (int j = 0 ; j<8; j++)
//        {
//            for (int k = 0; k<TUPLES_PER_BLOCK; k++)
//            {
//                sortVector.push_back({sortList[j][k*2], sortList[j][k*2+1]});
//
//            }
//        }
//        sort(sortVector.begin(), sortVector.end(), compare);
//        for (int j = 0 ; j<8; j++)
//        {
//            for(int k = 0; k<TUPLES_PER_BLOCK; k++)
//            {
//                sortStruct temp = sortVector.back();
//                sortVector.pop_back();
//                sortList[j][k*2] = temp.a;
//                sortList[j][k*2+1] = temp.b;
//
//            }
//            sortList[j][14] = 14;
//
//            if(j == 7)
//            {
//                sortList[j][15] = 0;
//            }
//            else
//                sortList[j][15] = nextDiskAddress+BLOCK_SIZE;
//            writeBlockToDisk((unsigned char*)sortList[j], nextDiskAddress, &buffer);
//            nextDiskAddress+=BLOCK_SIZE;
//
//        }
//        address.push_back(nextDiskAddress);
//
//    }
//
//
//    sortVector.clear();
//    for (int i = 0 ; i<S_BLOCK/8; i++)
//    {
//        for (int j = i*8; j<i*8+8; j++)
//        {
//            sortList[j-i*8] = (unsigned int*)readBlockFromDisk(S_ADDRESS+j*BLOCK_SIZE, &buffer);
//        }
//        for (int j = 0 ; j<8; j++)
//
//        {
//            for (int k = 0; k<TUPLES_PER_BLOCK; k++)
//            {
//                sortVector.push_back({sortList[j][k*2], sortList[j][k*2+1]});
//
//            }
//
//        }
//        sort(sortVector.begin(), sortVector.end(),compare);
//        for (int j = 0 ; j<8; j++)
//        {
//            for(int k = 0; k<TUPLES_PER_BLOCK; k++)
//            {
//                sortStruct temp = sortVector.back();
//                sortVector.pop_back();
//                sortList[j][k*2] = temp.a;
//                sortList[j][k*2+1] = temp.b;
//            }
//            sortList[j][14] = 14;
//
//            if(j == 7)
//            {
//                sortList[j][15] = 0;
//            }
//            else
//                sortList[j][15] = nextDiskAddress+BLOCK_SIZE;
//            writeBlockToDisk((unsigned char*)sortList[j], nextDiskAddress, &buffer);
//            nextDiskAddress+=BLOCK_SIZE;
//        }
//        address.push_back(nextDiskAddress);
//
//
//    }
//    address.pop_back();
//
//
////////////////////////////////////////////////////////////////
//    int pointer[6];
//
//    for (int i = 0; i<6; i++)
//    {
//        pointer[i] = 0;
//        blockList[i] = (unsigned int*)readBlockFromDisk(address[i], &buffer);
//    }
//    sorted_base_union = nextDiskAddress;
//    outputBuffer = (unsigned int*)getNewBlockInBuffer(&buffer);
//    outBufferInts = 0;
//    int beforeMin = 0;
//    int beforeMin2 = 0;
//    int curMin = 10000;
//    int curMin2 = 10000;
//    while(1)
//    {
//
//
//        if ((beforeMin2 != curMin2 || beforeMin != curMin) && curMin != 10000)
//        {
//            outputBuffer[outBufferInts] = curMin;
//            outputBuffer[outBufferInts+1] = curMin2;
//            outBufferInts += 2;
//            if (outBufferInts == 14)
//            {
//                outBufferInts = 0;
//                outputBuffer[14] = 14;
//                outputBuffer[15] = nextDiskAddress+BLOCK_SIZE;
//                writeBlockToDisk((unsigned char*)outputBuffer, nextDiskAddress, &buffer);
//                outputBuffer = (unsigned int*)getNewBlockInBuffer(&buffer);
//                nextDiskAddress += BLOCK_SIZE;
//
//            }
//            beforeMin2 = curMin2;
//            beforeMin = curMin;
//
//        }
//        if (blockList[0] == NULL &&blockList[1] == NULL &&blockList[2] == NULL &&blockList[3] == NULL &&blockList[4] == NULL &&blockList[5] == NULL)
//        {
//            break;
//        }
//
//        curMin = 10000;
//        curMin2 = 10000;
//        for (int i = 0; i<6; i++)
//        {
//            if (blockList[i] != NULL)
//            {
//                if (blockList[i][pointer[i]]<curMin)
//                {
//                    curMin = blockList[i][pointer[i]];
//                    curMin2 = blockList[i][pointer[i]+1];
//
//                }
//                else if(blockList[i][pointer[i]]==curMin && blockList[i][pointer[i]+1] < curMin2)
//                {
//                    curMin2 = blockList[i][pointer[i]+1];
//                }
//            }
//
//        }
//        for (int i = 0; i<6; i++)
//        {
//            if (blockList[i] != NULL)
//            {
//                if (blockList[i][pointer[i]] == curMin && blockList[i][pointer[i]+1] == curMin2)
//                {
//                    pointer[i] += 2;
//                    if (pointer[i] == 14)
//                    {
//
//                        if (blockList[i][15] == 0)
//                        {
//                            freeBlockInBuffer((unsigned char*)blockList[i], &buffer);
//                            blockList[i] = NULL;
//                            pointer[i] = 0;
//                        }
//                        else
//                        {
//
//                            nextBlockToRead = blockList[i][15];
//                            freeBlockInBuffer((unsigned char*)blockList[i], &buffer);
//                            blockList[i] = (unsigned int*)readBlockFromDisk(nextBlockToRead, &buffer);
//                            pointer[i] = 0;
//                        }
//                    }
//                }
//
//            }
//
//        }
//
//
//    }
//    if (outBufferInts!=0)
//    {
//        outputBuffer[14] = outBufferInts;
//        outputBuffer[15] = 0;
//        outBufferInts = 0;
//        writeBlockToDisk((unsigned char*)outputBuffer, nextDiskAddress, &buffer);
//        nextDiskAddress += BLOCK_SIZE;
//    }
//    else{
//        freeBlockInBuffer((unsigned char*)outputBuffer, &buffer);
//    }
//
//
//
//
////
//    //hash_join
//    unsigned int* hashList[7];
//    unsigned int nextDiskAddressList[7];
//    int pointerList[7];
//    int block_num_R[7];
//    int block_num_S[7];
//    for (int i = 0; i<7; i++)
//    {
//        hash_R[i] = nextDiskAddress+i*16*BLOCK_SIZE;
//        nextDiskAddressList[i] = hash_R[i];//***********************************
//        hash_S[i] = nextDiskAddress+7*16*BLOCK_SIZE+i*16*BLOCK_SIZE;
//        hashList[i] = (unsigned int*)getNewBlockInBuffer(&buffer);
//        pointerList[i] = 0;
//        block_num_R[i] = block_num_S[i] = 0;
//
//    }
//    nextDiskAddress += 14*16*BLOCK_SIZE;
//    for (int i = 0; i<R_BLOCK; i++)
//    {
//
//        block = (unsigned int*)readBlockFromDisk(R_ADDRESS+i*BLOCK_SIZE, &buffer);
//        for (int j = 0 ; j<TUPLES_PER_BLOCK; j++)
//        {
//            int curCanister = block[j*2]%7;
//            hashList[curCanister][pointerList[curCanister]++] = block[j*2];
//            hashList[curCanister][pointerList[curCanister]++] = block[j*2+1];
//            if (pointerList[curCanister] == 14)
//            {
//                hashList[curCanister][14] = 14;
//                hashList[curCanister][15] = nextDiskAddressList[curCanister]+BLOCK_SIZE;
//                writeBlockToDisk((unsigned char*)hashList[curCanister], nextDiskAddressList[curCanister], &buffer);
//                nextDiskAddressList[curCanister]+=BLOCK_SIZE;
//                hashList[curCanister] = (unsigned int*)getNewBlockInBuffer(&buffer);
//                pointerList[curCanister] = 0;
//                block_num_R[curCanister]++;
//            }
//        }
//        nextBlockToRead = block[15];
//        freeBlockInBuffer((unsigned char*)block, &buffer);
//
//    }
//    for (int i = 0; i<7; i++)
//    {
//
//        if(pointerList[i]!=0)
//        {
//            hashList[i][14] = pointerList[i];
//            hashList[i][15] = 0;
//            writeBlockToDisk((unsigned char*)hashList[i], nextDiskAddressList[i], &buffer);
//            block_num_R[i]++;
//        }
//        else
//        {
//            freeBlockInBuffer((unsigned char*)hashList[i], &buffer);
//            hashList[i] = (unsigned int*)readBlockFromDisk(nextDiskAddressList[i]-BLOCK_SIZE, &buffer);
//            hashList[i][15] = 0;
//            writeBlockToDisk((unsigned char*)hashList[i],nextDiskAddressList[i]-BLOCK_SIZE, &buffer );
//        }
//    }
//
//    for (int i = 0; i<7; i++)
//    {
//        nextDiskAddressList[i] = hash_S[i];//***********************************
//        hashList[i] = (unsigned int*)getNewBlockInBuffer(&buffer);
//        pointerList[i] = 0;
//
//    }
//
//    for (int i = 0; i<S_BLOCK; i++)
//    {
//        block = (unsigned int*)readBlockFromDisk(S_ADDRESS+i*BLOCK_SIZE, &buffer);
//        for (int j = 0 ; j<TUPLES_PER_BLOCK; j++)
//        {
//            int curCanister = block[j*2]%7;
//            hashList[curCanister][pointerList[curCanister]++] = block[j*2];
//            hashList[curCanister][pointerList[curCanister]++] = block[j*2+1];
//            if (pointerList[curCanister] == 14)
//            {
//                hashList[curCanister][14] = 14;
//                hashList[curCanister][15] = nextDiskAddressList[curCanister]+BLOCK_SIZE;
//                writeBlockToDisk((unsigned char*)hashList[curCanister], nextDiskAddressList[curCanister], &buffer);
//                nextDiskAddressList[curCanister]+=BLOCK_SIZE;
//                hashList[curCanister] = (unsigned int*)getNewBlockInBuffer(&buffer);
//                pointerList[curCanister] = 0;
//                block_num_S[curCanister]++;
//            }
//        }
//        nextBlockToRead = block[15];
//        freeBlockInBuffer((unsigned char*)block, &buffer);
//
//    }
//    for (int i = 0; i<7; i++)
//    {
//
//        if(pointerList[i]!=0)
//        {
//            hashList[i][14] = pointerList[i];
//            hashList[i][15] = 0;
//            writeBlockToDisk((unsigned char*)hashList[i], nextDiskAddressList[i], &buffer);
//            block_num_S[i]++;
//        }
//        else
//        {
//            freeBlockInBuffer((unsigned char*)hashList[i], &buffer);
//            hashList[i] = (unsigned int*)readBlockFromDisk(nextDiskAddressList[i]-BLOCK_SIZE, &buffer);
//            hashList[i][15] = 0;
//            writeBlockToDisk((unsigned char*)hashList[i],nextDiskAddressList[i]-BLOCK_SIZE, &buffer );
//        }
//    }
//
//    int hash_blocks = 0;
//    outputBuffer = (unsigned int*)getNewBlockInBuffer(&buffer);
//    outBufferInts = 0;
//    hash_join = nextDiskAddress;
//    for (int q = 0; q<7; q++)
//    {
//
//        for (int i = 0; i<block_num_R[q]; i++)
//        {
//            blockList[i%6] = (unsigned int*)readBlockFromDisk(hash_R[q]+i*BLOCK_SIZE, &buffer);
//            if (i == block_num_R[q]-1 || i%6 == 5)
//            {
//                for (int j = 0; j<block_num_S[q]; j++)
//                {
//                    block = (unsigned int*)readBlockFromDisk(hash_S[q]+j*BLOCK_SIZE, &buffer);
//                    for (int k = 0; k<block[14]/2; k++)
//                    {
//                        for (int x = 0; x<=i%6; x++)
//                        {
//                            for (int y = 0 ; y<blockList[x][14]/2; y++)
//                            {
//                                if (blockList[x][y*2] == block[k*2])
//                                {
//
//                                    outputBuffer[outBufferInts] = blockList[x][y*2];
//                                    outputBuffer[outBufferInts+1] = blockList[x][y*2+1];
//                                    outputBuffer[outBufferInts+2] = block[k*2];
//                                    outputBuffer[outBufferInts+3] = block[k*2+1];
//                                    outBufferInts+=4;
//
//                                    if (outBufferInts == 12)
//                                    {
//                                        outputBuffer[14] = 12;
//                                        outputBuffer[15] = nextDiskAddress+BLOCK_SIZE;
//                                        writeBlockToDisk((unsigned char*)outputBuffer, nextDiskAddress, &buffer );
//                                        nextDiskAddress += BLOCK_SIZE;
//                                        outputBuffer = (unsigned int*)getNewBlockInBuffer(&buffer);
//                                        outBufferInts = 0;
//                                        hash_blocks++;
//                                    }
//                                }
//                            }
//                        }
//                    }
//                    freeBlockInBuffer((unsigned char*)block, &buffer);
//
//                }
//                for (int j = 0; j<=i%6; j++)
//                {
//                    freeBlockInBuffer((unsigned char*)blockList[j], &buffer);
//                }
//            }
//        }
//    }
//
//    if(outBufferInts!=0)
//    {
//        hash_blocks++;
//        outputBuffer[14] = outBufferInts;
//        outputBuffer[15] = 0;
//        writeBlockToDisk((unsigned char*)outputBuffer, nextDiskAddress, &buffer);
//    }
//    else
//    {
//        freeBlockInBuffer((unsigned char*)outputBuffer, &buffer);
//        outputBuffer = (unsigned int*)readBlockFromDisk(nextDiskAddress-BLOCK_SIZE, &buffer);
//        outputBuffer[15] = 0;
//        writeBlockToDisk((unsigned char*)outputBuffer,nextDiskAddress-BLOCK_SIZE, &buffer );
//    }
//
//    cout<<"R:"<<endl;
//    test = (unsigned int*)readBlockFromDisk(R_ADDRESS, &buffer);
//    while(test[15]!=0)
//    {
//        for (int i = 0; i<test[14]/2; i++)
//        {
//            cout<<test[i*2]<<","<<test[i*2+1]<<"    ";
//        }
//        nextBlockToRead = test[15];
//        freeBlockInBuffer((unsigned char*)test, &buffer);
//        test = (unsigned int*)readBlockFromDisk(nextBlockToRead, &buffer);
//    }
//    for (int i = 0; i<test[14]/2; i++)
//    {
//        cout<<test[i*2]<<","<<test[i*2+1]<<"    ";
//    }
//    cout<<endl;
//    freeBlockInBuffer((unsigned char*)test, &buffer);
//
//
//    cout<<"S:"<<endl;
//    test = (unsigned int*)readBlockFromDisk(S_ADDRESS, &buffer);
//    while(test[15]!=0)
//    {
//        for (int i = 0; i<test[14]/2; i++)
//        {
//            cout<<test[i*2]<<","<<test[i*2+1]<<"    ";
//        }
//        nextBlockToRead = test[15];
//        freeBlockInBuffer((unsigned char*)test, &buffer);
//        test = (unsigned int*)readBlockFromDisk(nextBlockToRead, &buffer);
//    }
//    for (int i = 0; i<test[14]/2; i++)
//    {
//        cout<<test[i*2]<<","<<test[i*2+1]<<"    ";
//    }
//    cout<<endl;
//    freeBlockInBuffer((unsigned char*)test, &buffer);
//
//    //print selection result
//    cout<<"selection result:"<<endl;
//    test = (unsigned int*)readBlockFromDisk(selectionAddress, &buffer);
//    while(test[15]!=0)
//    {
//        for (int i = 0; i<test[14]/2; i++)
//        {
//            cout<<test[i*2]<<","<<test[i*2+1]<<"    ";
//        }
//        nextBlockToRead = test[15];
//        freeBlockInBuffer((unsigned char*)test, &buffer);
//        test = (unsigned int*)readBlockFromDisk(nextBlockToRead, &buffer);
//    }
//    for (int i = 0; i<test[14]/2; i++)
//    {
//        cout<<test[i*2]<<","<<test[i*2+1]<<"    ";
//    }
//    cout<<endl;
//    freeBlockInBuffer((unsigned char*)test, &buffer);
//
// //print projection result
//    cout<<"projection result:"<<endl;
//    test = (unsigned int*)readBlockFromDisk(projectionAddress, &buffer);
//    while(test[15]!=0)
//    {
//        for (int i = 0; i<test[14]; i++)
//        {
//            cout<<test[i]<<' ';
//        }
//        nextBlockToRead = test[15];
//        freeBlockInBuffer((unsigned char*)test, &buffer);
//        test = (unsigned int*)readBlockFromDisk(nextBlockToRead, &buffer);
//    }
//    for (int i = 0; i<test[14]; i++)
//    {
//        cout<<test[i]<<' ';
//    }
//    freeBlockInBuffer((unsigned char*)test, &buffer);
//    cout<<endl;
//
//
//
////print union result
//    cout<<"union"<<endl;
//    test = (unsigned int*)readBlockFromDisk(sorted_base_union, &buffer);
//    while(test[15]!=0)
//    {
//        for (int i = 0; i<test[14]/2; i++)
//        {
//            cout<<test[i*2]<<","<<test[i*2+1]<<"    ";
//        }
//        nextBlockToRead = test[15];
//        freeBlockInBuffer((unsigned char*)test, &buffer);
//        test = (unsigned int*)readBlockFromDisk(nextBlockToRead, &buffer);
//    }
//    for (int i = 0; i<test[14]/2; i++)
//    {
//        cout<<test[i*2]<<","<<test[i*2+1]<<"    ";
//    }
//    freeBlockInBuffer((unsigned char*)test, &buffer);
//    cout<<endl;
//
//    //print nest_loop_join result
//    int totalTuples = 0;
//    cout<<"nest_loop_join:"<<nest_blocks<<endl;
//    for (int i = 0; i<nest_blocks; i++)
//    {
//        test = (unsigned int*)readBlockFromDisk(i*64+nest_join, &buffer);
//        for (int i = 0; i<test[14]/2; i++)
//        {
//            cout<<test[i*2]<<","<<test[i*2+1]<<"    ";
//            totalTuples++;
//        }
//        freeBlockInBuffer((unsigned char*)test, &buffer);
//
//    }
//    cout<<endl;
//    cout<<"total tuples:"<<totalTuples<<endl;
//
//
//
//    //print hash join
//    totalTuples = 0;
//    cout<<"hash join:"<<endl;
//    for (int i = 0; i<hash_blocks; i++)
//    {
//        test = (unsigned int*)readBlockFromDisk(i*64+hash_join, &buffer);
//        for (int i = 0; i<test[14]/2; i++)
//        {
//            cout<<test[i*2]<<","<<test[i*2+1]<<"    ";
//            totalTuples++;
//        }
//        freeBlockInBuffer((unsigned char*)test, &buffer);
//
//    }
//    cout<<endl;
//    cout<<"total tuples:"<<totalTuples<<endl;





    return 0;
}
