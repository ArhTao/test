#ifndef ___MEM_POOL__H
#define ___MEM_POOL__H

#include "stdio.h"

typedef struct MemBlock
{
    struct MemBlock *next;
}MemBlock;

typedef struct mem_pool
{
    int block_size;
    int block_num;
    MemBlock * pool;
    MemBlock * freelist;
}Mempool;


Mempool *m_create(int block_size,int block_num);
void *m_malloc(Mempool * mp);
void m_free(Mempool * mp, void *ptr);
void m_destory(Mempool * mp);

#endif