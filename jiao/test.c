#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
/*
// 交操作
//核心函数1_4_2集合操作：交（R交S）
//每次装载NUMOFR(NUMOFR>=1 && NUMOFR<=6)块R，1块S，1块结果（读缓冲区取元组比较）
int AND4(Buffer buf) {
	unsigned char *Rblk[NUMOFR];		//存储NUMOFR块的R
	unsigned char *Sblk;				//存储1块S
	unsigned char *Result_blk;			//存储1块结果

	unsigned int R_next = 1;			//磁盘中R地址从1开始
	unsigned int S_next = 20;			//磁盘中S地址从20开始
	unsigned int Result_next = 1000;	//计划将结果存储在磁盘第1000块开始的位置

	int resultp = 0;					//存储结果块指针
	int A, B, C, D; //A的值域为[1, 40]，B的值域为[1, 1000] ，C的值域为[20, 60]，D的值域为[1, 1000]

	Result_blk = getNewBlockInBuffer(&buf);	//申请一块缓冲区存储临时结果
	memset(Result_blk, 0, 64);

	int turn = (16 % NUMOFR == 0) ? 16 / NUMOFR : 16 / NUMOFR + 1;		//有余数，需要多执行1轮

	for (int i = 0; i < turn; i++)			//R每次装载NUMOFR块
	{
		printf("这是第%d轮\n", i);

		int bn = 0;								//存储装载的R的块数
		for (bn, R_next; bn < NUMOFR && R_next != 0; bn++)	//一轮从磁盘读取NUMOFR个块的R到缓冲区，直到读完R
		{

			if ((Rblk[bn] = readBlockFromDisk(R_next, &buf)) == NULL)
			{
				perror("Reading R-Block Failed!\n");
				return -1;
			}
			printf("装载第%d块R\n", R_next);

			R_next = readNextAdd(Rblk[bn]);	//取下一块地址R_next

		}


		for (S_next = 20; S_next != 0;)			//S每次装载1块，直到装载完毕
		{

			if ((Sblk = readBlockFromDisk(S_next, &buf)) == NULL)
			{
				perror("Reading S-Block Failed!\n");
				return -1;
			}
			printf("装载第%d块S\n", S_next);
			S_next = readNextAdd(Sblk);

			//到此R和S装载好了
			//下面开始处理6块R和1块S的交，结果存在结果块
			for (int tRi = 0; tRi < bn; tRi++)
			{
				for (int tRj = 0; tRj < 7; tRj++)
				{
					readNextRecord(Rblk[tRi], tRj * 8, &A, &B);
					for (int tSj = 0; tSj < 7; tSj++)
					{
						readNextRecord(Sblk, tSj * 8, &C, &D);

						if (A == C && B == D)
						{
							printf("---第%d块的%d元组R和第%d块的%d元组S比较，(%d,%d),(%d,%d)\n", tRi, tRj, S_next, tSj, A, B, C, D);
							//找到符合交的数据，先写到Result_blk缓冲区
							writeRecord(Result_blk, resultp, A, B);
							resultp += 8;					//Result_blk缓冲区指针指向下一元组

							if (resultp == 56)				//结果写满一个块，将一个块写入磁盘
							{
								//printf("满一个块的结果，写磁盘\n");
								writeAdd(Result_blk, Result_next + 1);
								if (writeBlockToDisk(Result_blk, Result_next, &buf) != 0)		//写入磁盘
								{
									perror("Writing Block Failed!\n");
									return -1;
								}

								Result_next += 1;
								Result_blk = getNewBlockInBuffer(&buf);		//申请缓冲区新块
								memset(Result_blk, 0, 64);
								resultp = 0;
							}
						}
					}
				}
			}

			freeBlockInBuffer(Sblk, &buf);

		}

		//全部处理完毕后，释放缓冲区!
		for (int bl = 0; bl < NUMOFR; bl++)
		{
			freeBlockInBuffer(Rblk[bl], &buf);
		}
	}
	if (resultp != 0)			//如果还有未写入磁盘的结果，最后把结果块写入磁盘
	{
		writeAdd(Result_blk, Result_next + 1);
		if (writeBlockToDisk(Result_blk, Result_next, &buf) != 0)		//写入磁盘
		{
			perror("Writing Block Failed!\n");
			return -1;
		}
	}

	printf("\n\t注：结果写入磁盘：1000-%d\n\n", Result_next);
	printf("本次共发生%d次I/O\n", buf.numIO);

	return 0;
}

//
//int main(int argc, char **argv)
//{
//    Buffer buf; /* A buffer */
//    unsigned char *blk; /* A pointer to a block */
//    int i = 0;
//
//    /* Initialize the buffer */
//    if (!initBuffer(520, 64, &buf))
//    {
//        perror("Buffer Initialization Failed!\n");
//        return -1;
//    }
//
//    /* Get a new block in the buffer */
//    blk = getNewBlockInBuffer(&buf);
//
//    /* Fill data into the block */
//    for (i = 0; i < 8; i++)
//        *(blk + i) = 'a' + i;
//
//    /* Write the block to the hard disk */
//    if (writeBlockToDisk(blk, 8888, &buf) != 0)
//    {
//        perror("Writing Block Failed!\n");
//        return -1;
//    }
//
//    /* Read the block from the hard disk */
//    if ((blk = readBlockFromDisk(1, &buf)) == NULL)
//    {
//        perror("Reading Block Failed!\n");
//        return -1;
//    }
//
//    /* Process the data in the block */
//    int X = -1;
//    int Y = -1;
//    int addr = -1;
//
//    char str[5];
//    printf("block 1:\n");
//    for (i = 0; i < 7; i++) //一个blk存7个元组加一个地址
//    {
//
//        for (int k = 0; k < 4; k++)
//        {
//            str[k] = *(blk + i*8 + k);
//        }
//        X = atoi(str);
//        for (int k = 0; k < 4; k++)
//        {
//            str[k] = *(blk + i*8 + 4 + k);
//        }
//        Y = atoi(str);
//        printf("(%d, %d) ", X, Y);
//    }
//    for (int k = 0; k < 4; k++)
//    {
//        str[k] = *(blk + i*8 + k);
//    }
//    addr = atoi(str);
//    printf("\nnext address = %d \n", addr);
//
//
//    printf("\n");
//    printf("IO's is %d\n", buf.numIO); /* Check the number of IO's */
//
//    return 0;
//}


