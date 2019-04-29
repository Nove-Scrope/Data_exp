//#include <stdlib.h>
//#include <stdio.h>
//#include "extmem.h"
//
//int unionset(Buffer buf)
//{
//    unsigned char *Rblk[NUMOFR];
//    unsigned char *Sblk;
//    unsigned char *Result_blk;
//
//    unsigned int R_next = R_START_ADDRESS;
//    unsigned int S_next = S_START_ADDRESS;
//    unsigned int Result_next = 3000;
//
//    int resultp = 0;
//    int A, B, C, D;
//
//    Result_blk = getNewBlockInBuffer(&buf);
//
//    memset(Result_blk, 0, 64);
//
//    int turn = (16 % NUMOFR == 0) ? 16 / NUMOFR : 16 / NUMOFR + 1;
//
//    int it;
//    for(it = 0; it < turn; it++)
//    {
//        printf("This is %d cycle.\n", it);
//
//        int bn = 0;
//
//        for(bn, R_next; bn < NUMOFR && R_next != 0; bn ++)
//        {
//            if((Rblk[bn] = readBlockFromDisk(R_next, &buf)) == NULL)
//            {
//                perror("Reading R-Block Failed!\n");
//                return -1;
//            }
//            printf("Loading %dth R-BLock.\n", R_next);
//
//            R_next = readNextBlockAddr(Rblk[bn]);
//        }
//
//
//        int flag[6][7] = {0};
//
//        for(S_next = 20; S_next != 0;)
//        {
//            if ((Sblk = readBlockFromDisk(S_next, &buf)) == NULL)
//			{
//				perror("Reading S-Block Failed!\n");
//				return -1;
//			}
//			printf("装载第%d块S\n", S_next);
//
//            int tRi, tRj, tSk;
//            for(tRi = 0; tRi < bn; tRi ++)
//            {
//                for(tRj = 0; tRj < 7; tRj ++)
//                {
//                    readNextRecord(Rblk[tRi], tRj, &A, &B);
//                    for(tSk = 0; tSk < 7; tSk ++)
//                    {
//                        readNextRecord(Sblk, tSk, &C, &D);
//                        if(A == C && B== D)
//                            flag[tRi][tRj] = 1;
//                    }
//
//                }
//            }
//
//			S_next = readNextBlockAddr(Sblk);
//			freeBlockInBuffer(Sblk, &buf);
//        }
//
//        int i, j;
//        for(i = 0; i < bn; i ++)
//        {
//            for(j = 0; j < 7; j ++)
//            {
//                if(!flag[i][j])
//                {
//                    readNextRecord(Rblk[i], j, &A, &B);
//                    writeRecord(Rblk[i], resultp, A, B);
//                    resultp += 1;
//
//                    if( resultp == 7)
//                    {
//
//                        writeRecord(Result_blk, 7, Result_next + 1, 0);
//                        if(writeBlockToDisk(Result_blk, Result_next, &buf) != 0)
//                        {
//                            perror("Writing Block Failed!\n");
//                            return -1;
//                        }
//                        Result_next ++;
//                        Result_blk = getNewBlockInBuffer(&buf);
//                        memset(Result_blk, 0, 64);
//                        resultp = 0;
//                    }
//                }
//            }
//
//        }
//
//        int bl;
//        for(bl = 0; bl < bn; bl ++)
//        {
//            freeBlockInBuffer(Rblk[bl], &buf);
//        }
//
//    }
//
//
//    if(resultp != 0)
//    {
//        writeRecord(Result_blk, 7, 0, 0);
//        if(writeBlockToDisk(Result_blk, Result_next, &buf) != 0)
//        {
//            perror("Writing Block Failed!\n");
//            return -1;
//        }
//    }
//
//
//    printf("\nTip:Result are writing to disk 2000-%d\n\n", Result_next);
//    printf("IO times: %d\n", buf.numIO);
//    return 0;
//}
