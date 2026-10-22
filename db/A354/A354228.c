/*
 * A354228  Number of partitions of the multigraph G_n (defined below) into
 *          "strokes".
 * https://oeis.org/A354228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 22*a(n-2) - 88*a(n-3) + 112*a(n-4) * Initial
 *            values: a(1)=1, a(2)=6, a(3)=58, a(4)=578, a(5)=5766 * * The
 *            function is memoised and works for any n >= 1. * It uses only
 *            integer arithmetic and no external libraries
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <vector>
#endif
#include <stdlib.h>   /* malloc, realloc, free in C */

/* static cache for computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;    /* allocated size of cache array */
static idx_t  cache_max = 0;    /* highest index that has been computed */

/* enlarge cache array to hold at least (n+1) elements */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;   /* already large enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap <= (size_t)n)
        new_cap = (size_t)n + 1;

#if defined(__cplusplus)
    /* allocate new array and copy old contents */
    seq_t *new_buf = new seq_t[new_cap];
    for (size_t i = 0; i < cache_cap; ++i)
        new_buf[i] = cache[i];
    delete[] cache;
    cache = new_buf;
#else
    seq_t *new_buf = (seq_t *)realloc(cache,
                                      new_cap * sizeof(seq_t));
    cache = new_buf;
#endif
    cache_cap = new_cap;
    /* initialise the new part to 0 (not strictly needed for idx 0) */
    if (cache_cap > 0)
        cache[0] = (seq_t)0;
}

/* initialise cache with the first five terms of the sequence */
static void init_cache(void)
{
    static int inited = 0;
    if (inited)
        return;
    inited = 1;

    ensure_cache(5);          /* need indices 1..5 */
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)6;
    cache[3] = (seq_t)58;
    cache[4] = (seq_t)578;
    cache[5] = (seq_t)5766;
    cache_max = 5;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* out of offset, harmless */

    init_cache();

    if (n <= cache_max)
        return cache[n];

    ensure_cache(n);
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* use an expression that keeps the intermediate result non‑negative
           for all n >= 6 (verified for the first few terms) */
        seq_t val = (seq_t)13 * cache[i - 1]
                  + (seq_t)112 * cache[i - 4]
                  - (seq_t)22 * cache[i - 2]
                  - (seq_t)88 * cache[i - 3];
        cache[i] = val;
    }
    cache_max = n;
    return cache[n];
}
