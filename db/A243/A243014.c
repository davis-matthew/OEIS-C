/*
 * A243014  Number of acyclic digraphs (DAGS) on n labeled nodes, where the
 *          indegree and outdegree of each node is at most 1.
 * https://oeis.org/A243014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=1, a(1)=1; a(n)=n*(a(n-1)+n-2)+1 for n>1
 * Implements A243014: Number of acyclic digraphs on n labeled nodes with indegree,outdegree ≤1
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0; /* allocated capacity */
    static idx_t cache_len   = 0; /* number of valid entries, < cache_cap */

    /* ensure at least n+1 capacity */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = NULL;
        if (cache == NULL)
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the base terms when the cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        if (cache_cap >= 2)
            cache[1] = (seq_t)1;
        cache_len = (cache_cap >= 2) ? 2 : 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = (seq_t)1;
            continue;
        }
        seq_t sum   = cache[i - 1] + (seq_t)(i - 2);
        seq_t term  = (seq_t)i * sum + (seq_t)1;
        cache[i]    = term;
    }
    cache_len = n + 1;

    return cache[n];
}
