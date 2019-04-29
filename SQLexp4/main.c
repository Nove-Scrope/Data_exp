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


//    unsigned char *result;
//    int test = 1200;
//    int next_id = test;
//    result = readBlockFromDisk(test,&buf);
//    for(next_id = test; next_id != 0;)
//    {
//        result = readBlockFromDisk(next_id, &buf);
//        showBlockData(result);
//        next_id = readNextBlockAddr(result);
//        freeBlockInBuffer(result, &buf);
//    }


//    linear(buf);
//
//    projection(buf);
//
//
//    intersection(buf);
//    unionset(buf);
//    difference(buf);

    return 0;
}



/* The interection set  of R and S*/
//   intersection(buf);
/* The union set of R and S*/
//    unionset(buf);
/* The difference set of R and S*/
//  difference(buf);
