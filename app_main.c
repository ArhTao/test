#include "stdio.h"
#include "inc/mem_pool.h"

int main()
{
    printf("hello world \n");

    Mempool *pool = m_create(16, 5);

    void *p1 = m_malloc(pool);
    void *p2 = m_malloc(pool);
    printf("p1=%p  p2=%p\n", p1, p2);

    m_free(pool, p1);
    m_free(pool, p2);

    m_destory(pool);
    return 0;
}
