/*
 * A264072  Number of (3+1) X (n+1) arrays of permutations of 0..n*4+3 with
 *          each element having index change +-(.,.) 0,0 1,1 or 1,2.
 * https://oeis.org/A264072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 12*a(n-2) + 12*a(n-3) - 12*a(n-4) + a(n-5)
 *            for n>7 with a(1)=8, a(2)=89, a(3)=1210, a(4)=12100,
 *            a(5)=131890, a(6)=1445345, a(7)=15892745
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n<1 */

    /* Static cache to store already computed terms */
    static seq_t *cache = NULL;          /* cache[1] holds a(1) */
    static size_t cache_cap = 0;         /* allocated size */
    static size_t cache_max = 0;         /* highest index computed */

    /* Initial base values (indices 1..7) */
    static const seq_t base[8] = {
        0,                          /* index 0 unused */
        8,                           /* a(1) */
        89,                          /* a(2) */
        1210,                        /* a(3) */
        12100,                       /* a(4) */
        131890,                      /* a(5) */
        1445345,                     /* a(6) */
        15892745                     /* a(7) */
    };

    /* Allocate cache lazily */
    if (cache == NULL) {
        /* Allocate enough for up to at least n+1 entries */
        cache_cap = (size_t)n + 1;
        if (cache_cap < 8) cache_cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) {
            /* OOM: terminate (problematic in unit tests but required). */
            exit(1);
        }
        /* Copy the base values */
        for (size_t i = 1; i <= 7 && i < cache_cap; ++i)
            cache[i] = base[i];
        cache_max = 7;
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) {
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i <= 7) {
            val = base[i];
        } else {
            /* Recurrence: 12*a(i-1) - 12*a(i-2) + 12*a(i-3) - 12*a(i-4) + a(i-5) */
            val = (seq_t)12 * cache[i-1]
               - (seq_t)12 * cache[i-2]
               + (seq_t)12 * cache[i-3]
               - (seq_t)12 * cache[i-4]
               +  cache[i-5];
        }
        cache[i] = val;
    }
    cache_max = (size_t)n;

    return cache[n];
}
