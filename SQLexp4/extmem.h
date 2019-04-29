/*
 * extmem.h
 * Zhaonian Zou
 * Harbin Institute of Technology
 * Jun 22, 2011
 */

#ifndef EXTMEM_H
#define EXTMEM_H

#define BLOCK_AVAILABLE 0
#define BLOCK_UNAVAILABLE 1

#define BUFSIZE 520
#define BLKSIZE 64

#define NUMOFR 6
#define FREEBLOCK 7
#define R_START_ADDRESS 1
#define S_START_ADDRESS 20


typedef struct tagBuffer {
    unsigned long numIO; /* Number of IO's*/
    size_t bufSize; /* Buffer size*/
    size_t blkSize; /* Block size */
    size_t numAllBlk; /* Number of blocks that can be kept in the buffer */
    size_t numFreeBlk; /* Number of available blocks in the buffer */
    unsigned char *data; /* Starting address of the buffer */
} Buffer;

/* Initialize a buffer with the specified buffer size and block size.
 * If the initialization fails, the return value is NULL;
 * otherwise the pointer to the buffer.
 */
 /*初始化缓冲区*/
Buffer *initBuffer(size_t bufSize, size_t blkSize, Buffer *buf);

/* Free the memory used by a buffer. */
/*释放缓冲区buf占用的内存空间*/
void freeBuffer(Buffer *buf);

/* Apply for a new block from a buffer.
 * If no free blocks are available in the buffer,
 * then the return value is NULL; otherwise the pointer to the block.
 */
 /*在缓冲区申请一个新的块*/
unsigned char *getNewBlockInBuffer(Buffer *buf);

/* Set a block in a buffer to be available. */
/*解除对缓冲区内存的占用（将blk的内存区域标记为可用）*/
void freeBlockInBuffer(unsigned char *blk, Buffer *buf);

/* Drop a block on the disk */
/*从磁盘上删除地址为addr的磁盘块内的数据*/
int dropBlockOnDisk(unsigned int addr);

/* Read a block from the hard disk to the buffer by the address of the block. */
/*将磁盘上地址为addr的一个块读入缓冲区*/
unsigned char *readBlockFromDisk(unsigned int addr, Buffer *buf);

/* Read a block in the buffer to the hard disk by the address of the block. */
/*将缓冲区buf内的块blk写入磁盘上地址为addr的磁盘块*/
int writeBlockToDisk(unsigned char *blkPtr, unsigned int addr, Buffer *buf);




/* Print the data of the current block*/
void showBlockData(unsigned int *blk);

/* Read the address of the next block*/
int readNextBlockAddr(unsigned int *blk);

/* Read the record in the block*/
int readNextRecord(unsigned int *blk, unsigned int offset, int *A, int *B);


int writeRecord(unsigned int *blk, int offset, int A, int B);

int writeRestToDisk(unsigned char *blk, int next, Buffer *buf);

int writeOneRecord(unsigned int *blk, int offset, int A);

#endif // EXTMEM_H

