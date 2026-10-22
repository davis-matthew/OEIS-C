/*
 * A331725  E.g.f.: exp(x/(1 - x)) / (1 + x).
 * https://oeis.org/A331725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) * (a(n-1) + (n+1)*a(n-2) - (n-2)^2*a(n-3)), a(0)=1,
 *            a(1)=0, a(2)=3, a(3)=4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is A331725, OEIS. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free, realloc, exit */

#ifdef __cplusplus
/* In C++ build seq_t is an arithmetic type or a class with operator overloads. */
#else
/* In C build seq_t is a builtin unsigned integer type. */
#endif

static seq_t *cache = NULL;      /* static cache for memoisation */
static size_t cache_cap = 0;     /* allocated size of cache */

/* Ensure that the cache has room for indices 0..need-1.  Grow if necessary. */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need) return;   /* already big enough */

    /* Allocate new array (use operator new in C++ to give proper construction) */
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[need];
#else
    new_cache = (seq_t *)malloc(need * sizeof(seq_t));
    if (!new_cache) exit(1);
#endif

    /* Copy old data if present */
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
#ifndef __cplusplus
        free(cache);
#else
        delete[] cache;
#endif
    }

    cache = new_cache;
    cache_cap = need;
}

/* Initialise cache with known first terms */
static void init_cache(void)
{
    if (cache_cap != 0) return;   /* already initialised */

    ensure_cache(4);              /* need indices 0..3 */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)4;
}

/* Main sequence function, with static memoisation. */
seq_t a(idx_t n) {
    /* handle small indices directly to avoid loop overhead */
    if (n < 3) {
        /* 0, 1, 2, 3 handled by initialized cache */
    }

    /* Make sure the cache is ready */
    init_cache();

    if ((size_t)n < cache_cap)     /* already computed */
        return cache[(size_t)n];

    /* Need to compute up to n */
    ensure_cache((size_t)n + 1);   /* need space for indices 0..n */

    /* Compute missing terms */
    for (idx_t i = 4; i <= n; ++i) {
        /* Use size_t for array index */
        size_t idx = (size_t)i;

        seq_t term1 = cache[idx - 1];
        seq_t term2 = cache[idx - 2] * (seq_t)(i + 1);

        idx_t sq = i - 2;                     /* (i-2) */
        seq_t term3 = cache[idx - 3] * (seq_t)(sq * sq);

        seq_t sum = term1 + term2 - term3;
        cache[idx] = (seq_t)(i - 1) * sum;
    }

    return cache[(size_t)n];
}
