#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
/*
// ������
//���ĺ���1_4_2���ϲ���������R��S��
//ÿ��װ��NUMOFR(NUMOFR>=1 && NUMOFR<=6)��R��1��S��1��������������ȡԪ��Ƚϣ�
int AND4(Buffer buf) {
	unsigned char *Rblk[NUMOFR];		//�洢NUMOFR���R
	unsigned char *Sblk;				//�洢1��S
	unsigned char *Result_blk;			//�洢1����

	unsigned int R_next = 1;			//������R��ַ��1��ʼ
	unsigned int S_next = 20;			//������S��ַ��20��ʼ
	unsigned int Result_next = 1000;	//�ƻ�������洢�ڴ��̵�1000�鿪ʼ��λ��

	int resultp = 0;					//�洢�����ָ��
	int A, B, C, D; //A��ֵ��Ϊ[1, 40]��B��ֵ��Ϊ[1, 1000] ��C��ֵ��Ϊ[20, 60]��D��ֵ��Ϊ[1, 1000]

	Result_blk = getNewBlockInBuffer(&buf);	//����һ�黺�����洢��ʱ���
	memset(Result_blk, 0, 64);

	int turn = (16 % NUMOFR == 0) ? 16 / NUMOFR : 16 / NUMOFR + 1;		//����������Ҫ��ִ��1��

	for (int i = 0; i < turn; i++)			//Rÿ��װ��NUMOFR��
	{
		printf("���ǵ�%d��\n", i);

		int bn = 0;								//�洢װ�ص�R�Ŀ���
		for (bn, R_next; bn < NUMOFR && R_next != 0; bn++)	//һ�ִӴ��̶�ȡNUMOFR�����R����������ֱ������R
		{

			if ((Rblk[bn] = readBlockFromDisk(R_next, &buf)) == NULL)
			{
				perror("Reading R-Block Failed!\n");
				return -1;
			}
			printf("װ�ص�%d��R\n", R_next);

			R_next = readNextAdd(Rblk[bn]);	//ȡ��һ���ַR_next

		}


		for (S_next = 20; S_next != 0;)			//Sÿ��װ��1�飬ֱ��װ�����
		{

			if ((Sblk = readBlockFromDisk(S_next, &buf)) == NULL)
			{
				perror("Reading S-Block Failed!\n");
				return -1;
			}
			printf("װ�ص�%d��S\n", S_next);
			S_next = readNextAdd(Sblk);

			//����R��Sװ�غ���
			//���濪ʼ����6��R��1��S�Ľ���������ڽ����
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
							printf("---��%d���%dԪ��R�͵�%d���%dԪ��S�Ƚϣ�(%d,%d),(%d,%d)\n", tRi, tRj, S_next, tSj, A, B, C, D);
							//�ҵ����Ͻ������ݣ���д��Result_blk������
							writeRecord(Result_blk, resultp, A, B);
							resultp += 8;					//Result_blk������ָ��ָ����һԪ��

							if (resultp == 56)				//���д��һ���飬��һ����д�����
							{
								//printf("��һ����Ľ����д����\n");
								writeAdd(Result_blk, Result_next + 1);
								if (writeBlockToDisk(Result_blk, Result_next, &buf) != 0)		//д�����
								{
									perror("Writing Block Failed!\n");
									return -1;
								}

								Result_next += 1;
								Result_blk = getNewBlockInBuffer(&buf);		//���뻺�����¿�
								memset(Result_blk, 0, 64);
								resultp = 0;
							}
						}
					}
				}
			}

			freeBlockInBuffer(Sblk, &buf);

		}

		//ȫ��������Ϻ��ͷŻ�����!
		for (int bl = 0; bl < NUMOFR; bl++)
		{
			freeBlockInBuffer(Rblk[bl], &buf);
		}
	}
	if (resultp != 0)			//�������δд����̵Ľ�������ѽ����д�����
	{
		writeAdd(Result_blk, Result_next + 1);
		if (writeBlockToDisk(Result_blk, Result_next, &buf) != 0)		//д�����
		{
			perror("Writing Block Failed!\n");
			return -1;
		}
	}

	printf("\n\tע�����д����̣�1000-%d\n\n", Result_next);
	printf("���ι�����%d��I/O\n", buf.numIO);

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
//    for (i = 0; i < 7; i++) //һ��blk��7��Ԫ���һ����ַ
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

