#include "../inc/mem_pool.h"
#include <stdlib.h>

/**         初始化内存池
 *         ┌-------------┐
 *         │  free_list  │──┐
 *         └-------------┘  │
 *                          ▼
 *              -------------------------------------------------------------
 *              |        ┌---------┐   ┌---------┐   ┌---------┐            |
 *              |        │ block1  │──▶│ block2  │──▶│ block3  │──▶ NULL  |    POOL
 *              |        └---------┘   └---------┘   └---------┘            |
 *              |                                                           |          
 *              -------------------------------------------------------------
 * 
 **/

Mempool *m_create(int block_size,int block_num)
{
    Mempool *mp = (Mempool* )malloc(sizeof(Mempool));
    if(mp == NULL)
        return NULL;

    mp->block_size = block_size;
    mp->block_num = block_num;
    mp->pool = NULL;
    mp->freelist = NULL;
    
    MemBlock *chunk = (MemBlock *)malloc(block_size * block_num);
    if(chunk == NULL)
        return NULL;
    
    mp->pool = chunk;
    MemBlock *cur = chunk;
    for(int i = 1; i < block_num; i++)
    {
        cur->next = (MemBlock *)((char *)cur + block_size);
        cur = cur->next;
    }
    cur->next = NULL;
    mp->freelist = chunk;
    return mp;
}


/**
 *          m_malloc() 后：
 *         ┌-------------┐
 *         │  free_list  │──┐
 *         └-------------┘  │
 *                          ▼
 *                        ┌---------┐   ┌---------┐
 *                        │ block2  │──▶│ block3  │──▶ NULL
 *                        └---------┘   └---------┘
 *          返回 block1 给用户
 * 
 *          ┌-------------┐
 *          │  free_list  │──┐
 *          └-------------┘  │
 *                           ▼
 *                        ┌---------┐
 *                        │new chunk│──▶ NULL
 *                        └---------┘
 * 
 *         当前freelist指向NULL时，扩容重新malloc内存池后，mp->freelist指向新chunk;
 * 
**/

void *m_malloc(Mempool * mp)
{
    if(!mp->freelist)
    {
        const size_t new_blocks = 10;
        MemBlock *chunk = (MemBlock *)malloc(mp->block_size * new_blocks);
        if(chunk == NULL)
            return NULL;

        MemBlock *cur = chunk;
        for (size_t i = 1; i < new_blocks; i++) {
            cur->next = (MemBlock*) ((char*)cur + mp->block_size);
            cur = cur->next;
        }
        cur->next     = NULL;
        mp->freelist  = chunk;
    }
    MemBlock * block = mp->freelist;
    mp->freelist = block->next;
    return block;
}

/**
 *       ┌---------┐   
 *       │ block1  │──▶  
 *       └---------┘   |
 *                     ▼  
 *            ┌-------------┐
 *            │  free_list  │──┐
 *            └-------------┘  │
 *                             ▼  
 *                      ┌---------┐   ┌---------┐
 *                      │ block2  │──▶│ block3  │──▶ NULL
 *                      └---------┘   └---------┘
 * 
 * 
*/

void m_free(Mempool * mp, void *ptr)
{
    if(!ptr)
        return;
    MemBlock *block = (MemBlock*)ptr;
    block->next = mp->freelist;
    mp->freelist = block;
}


void m_destory(Mempool * mp)
{
    if(!mp)
        return;
    free(mp->pool);
    free(mp);
}




 