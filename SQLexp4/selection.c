#include <stdio.h>
#include <stdlib.h>
#include "extmem.h"

int linear(Buffer buf)
{
    unsigned char *Rblk;
    unsigned char *Sblk;
    unsigned char *Result_blk;

    unsigned int R_next = R_START_ADDRESS;
    unsigned int S_next = S_START_ADDRESS;
    unsigned int Result_next = 200;

    int resultp = 0;
    int A, B, C, D;

    Result_blk = getNewBlockInBuffer(&buf);

    memset(Result_blk, 0, 64);

    for(R_next = R_START_ADDRESS; R_next != 0;)
    {
        Rblk = readBlockFromDisk(R_next, &buf);

        int i = 7;
        for(i = 0; i < 7; i ++)
        {
            readNextRecord(Rblk, i, &A, &B);
            if(A == 40)
            {
                writeRecord(Result_blk, resultp, A, B);
                resultp ++;
                if( resultp == 7)
                {
                    writeRecord(Result_blk, 7, Result_next + 1, 0);
                    if(writeBlockToDisk(Result_blk, Result_next, &buf) != 0)
                    {
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    Result_next ++;
                    Result_blk = getNewBlockInBuffer(&buf);
                    memset(Result_blk, 0, 64);
                    resultp = 0;
                }
            }
        }

        R_next = readNextBlockAddr(Rblk);
        freeBlockInBuffer(Rblk, &buf);
    }


    for(S_next = S_START_ADDRESS; S_next != 0;)
    {
        Sblk = readBlockFromDisk(S_next, &buf);

        int i = 7;
        for(i = 0; i < 7; i ++)
        {
            readNextRecord(Sblk, i, &C, &D);
            if(C == 40)
            {
                printf("%d %d\n", C, D);
                writeRecord(Result_blk, resultp, C, D);
                resultp ++;
                if(resultp == 7)
                {
                    writeRecord(Result_blk, 7, Result_next + 1, 0);
                    if(writeBlockToDisk(Result_blk, Result_next, &buf) != 0)
                    {
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    Result_next ++;
                    Result_blk = getNewBlockInBuffer(&buf);
                    memset(Result_blk, 0, 64);
                    resultp = 0;
                }
            }
        }

        S_next = readNextBlockAddr(Sblk);
        freeBlockInBuffer(Sblk, &buf);
    }

    if(resultp != 0)
        writeRestToDisk(Result_blk, Result_next, &buf);

    printf("\nTip:Result are writing to disk 200-%d\n\n", Result_next);
    printf("IO times: %lu\n", buf.numIO);
    return 0;
}


