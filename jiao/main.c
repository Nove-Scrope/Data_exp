#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extmem.h"

int main()
{
        Buffer buf;
        if (initBuffer(BUFSIZE, BLKSIZE, &buf) == NULL)
        {
            printf("Failed to initial buffer.\n");
        }
        else
        {
            printf("Success to initial buffer.\n");
        }
        ADD4(buf);

        return 0;
}

int ADD4(Buffer buf)
{
        unsigned char *Rblk[NUMOFR];
        unsigned char *Sblk;
        unsigned char *Result_blk;

        unsigned int R_next = R_START_ADDRESS;
        unsigned int S_next = S_START_ADDRESS;
        unsigned int Result_next = 1000;

        int resultp = 0;
        int A, B, C, D;

        Result_blk = getNewBlockInBuffer(&buf);

        memset(Result_blk, 0, 64);

        int turn = (16 % NUMOFR == 0)? 16 / NUMOFR : 16 / NUMOFR + 1;

        unsigned int iturn;
        for(iturn = 0; iturn < turn; iturn++)
        {
            printf("This is %dth cycle.\n", iturn+ 1);

            int bn = 0;
            for(bn, R_next; bn < NUMOFR  && R_next != 0; bn ++)
            {
                if((Rblk[bn] = readBlockFromDisk(R_next, &buf)) == NULL)
                {
                    perror("Reading R-block Failed!\n");
                    return -1;
                }
    //            printf("Loading the %dth R-Block\n", R_next);
                R_next = readNextBlockAddr(Rblk[bn]);
    //            printf("%d\t", R_next);
            }

            for(S_next = S_START_ADDRESS; S_next != 0;)
            {
                if((Sblk = readBlockFromDisk(S_next, &buf)) == NULL)
                {
                    perror("Reading S-Block Failed!\n");
                    return -1;
                }
    //            printf("Loading the %dth S-Block\n", S_next);
                S_next = readNextBlockAddr(Sblk);

                unsigned int tRi, tRj, tRk;

                for(tRi = 0; tRi < bn; tRi ++)
                {
                    for(tRj = 0; tRj < 7; tRj ++)
                    {
                        readNextRecord(Rblk[tRi], tRj, &A, &B);

                        for(tRk = 0; tRk < 7; tRk ++)
                        {
                            readNextRecord(Sblk, tRk, &C, &D);
                            if(A==C && B==D)
                            {
                                printf("---Compare R%d-Block%d with S%d-Block%d, (%d, %d), (%d, %d)\n",
                                    tRi, tRj, S_next, tRk, A, B, C, D);
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
                                    printf("%d\n", Result_next);
                                }


                        }
                    }
                }

                freeBlockInBuffer(Sblk, &buf);
            }

            unsigned int bl;
            for(bl = 0; bl < NUMOFR; bl ++)
            {
                freeBlockInBuffer(Rblk[bl], &buf);
            }
        }

            if(resultp != 0)
            {
                writeRecord(Result_blk, 7, Result_next + 1, 0);
                if(writeBlockToDisk(Result_blk, Result_next, &buf) != 0)
                {
                    perror("Writing Block Failed!\n");
                    return -1;
                }
            }


        printf("\nTip:Result are writing to disk 1000-%d\n\n", Result_next);
        printf("IO times: %d\n", buf.numIO);
        return 0;
}

