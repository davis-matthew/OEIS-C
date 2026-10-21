/*
 * A256316  Number of partitions of 4n into exactly 5 parts.
 * https://oeis.org/A256316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 468, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A256316
    Number of partitions of 4*n into exactly 5 parts.
    Recurrence (order 11):
      a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 3*a(n-4) + 4*a(n-5)
           - 4*a(n-6) + 3*a(n-7) - 2*a(n-8) + 3*a(n-9) - 3*a(n-10) + a(n-11)
    Initial values:
      a(0)=0, a(1)=0, a(2)=3, a(3)=13, a(4)=37, a(5)=84,
      a(6)=164, a(7)=291, a(8)=480, a(9)=748, a(10)=1115. */

#ifndef __cplusplus
#include <stdlib.h>
#include "../utils/seq_free.h"
#endif

/* Memory helpers that work both in C and C++ */
#ifdef __cplusplus
static seq_t* seq_malloc(size_t n)
{
    return new seq_t[n]();          /* value‑initialise to zero */
}

static seq_t* seq_realloc(seq_t* oldp, size_t oldlen, size_t newlen)
{
    seq_t* newp = new seq_t[newlen]();
    for (size_t i = 0; i < oldlen; ++i)
        newp[i] = oldp[i];
    delete [] oldp;
    return newp;
}
#else
static seq_t* seq_malloc(size_t n)
{
    return (seq_t*)calloc(n, sizeof(seq_t));   /* zero‑init */
}

static seq_t* seq_realloc(seq_t* oldp, size_t oldlen, size_t newlen)
{
    seq_t* newp = (seq_t*)realloc(oldp, newlen * sizeof(seq_t));
    return newp ? newp : oldp;                /* on realloc failure return oldp */
}
#endif

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* current allocated size (in entries) */

    /* number of initial terms known a priori (indices 0..10) */
    const size_t INIT = 11;

    if (cache == NULL) {
        /* initialise cache with space for indices 0..10 */
        cache_len = INIT;
        cache = seq_malloc(cache_len);
        if (!cache) return 0;  /* allocation failed – return zero */

        /* set the 11 known terms */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 3;
        cache[3] = 13;
        cache[4] = 37;
        cache[5] = 84;
        cache[6] = 164;
        cache[7] = 291;
        cache[8] = 480;
        cache[9] = 748;
        cache[10] = 1115;
    }

    /* if the requested index is already in the cache, return it */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* expand the cache to accommodate index n */
    size_t old_len = cache_len;
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = seq_realloc(cache, old_len, new_len);
    if (tmp == NULL) {          /* realloc failed – keep old cache */
        return cache[n];        /* value will be computed later */
    }
    cache = tmp;
    cache_len = new_len;

    /* compute missing entries using the recurrence */
    for (size_t i = old_len; i <= (size_t)n; ++i) {
        if (i < INIT) continue;              /* base terms already set */

        seq_t pos = 3 * cache[i-1] +
                    2 * cache[i-3] +
                    4 * cache[i-5] +
                    3 * cache[i-7] +
                    3 * cache[i-9] +
                    cache[i-11];

        seq_t neg = 3 * cache[i-2] +
                    3 * cache[i-4] +
                    4 * cache[i-6] +
                    2 * cache[i-8] +
                    3 * cache[i-10];

        cache[i] = pos - neg;     /* safe: pos > neg for all valid n */
    }

    return cache[n];
}
