#include <stdio.h>
#include <stdlib.h>
#include "extmem.h"

int projection(Buffer buf)
{
    unsigned char *Rblk;
    unsigned char *Result_blk;
    unsigned char *Temp_blk;

    unsigned int R_next = R_START_ADDRESS;
    unsigned int Result_next = 300;

    int resultp = 0;
    int A, B;

    Result_blk = getNewBlockInBuffer(&buf);

    memset(Result_blk, 0, 64);

    for(R_next = R_START_ADDRESS; R_next != 0;)
    {
        Rblk = readBlockFromDisk(R_next, &buf);

        int i;
        for(i = 0; i < 7; i ++)
        {
            readNextRecord(Rblk, i, &A, &B);
            writeOneRecord(Result_blk, resultp, A);
            resultp ++;
            if( resultp == 14)
            {
                writeRecord(Result_blk, 7, Result_next + 1, 0);
                if(writeBlockToDisk(Result_blk, Result_next, &buf) != 0)
                {
                    perror("Writing Block Failed!\n");
                    return -1;
                }
                Result_next ++;
                Temp_blk = Result_blk;
                Result_blk = getNewBlockInBuffer(&buf);
                memset(Result_blk, 0, 64);
                resultp = 0;
            }
        }

        R_next = readNextBlockAddr(Rblk);
        freeBlockInBuffer(Rblk, &buf);
    }


    if(resultp != 0)
        writeRestToDisk(Result_blk, Result_next, &buf);
    else
    {
        Temp_blk = readBlockFromDisk(Result_next - 1, &buf);
        writeRestToDisk(Temp_blk, Result_next, &buf);
    }

    printf("\nTip:Result are writing to disk 300-%d\n\n", Result_next);
    printf("IO times: %lu\n", buf.numIO);

    return 0;
}

