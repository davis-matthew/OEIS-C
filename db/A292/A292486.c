/*
 * A292486  p-INVERT of the odd positive integers, where p(S) = 1 - S - 3 S^2.
 * https://oeis.org/A292486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) + 9*a(n-3) + 3*a(n-4) for n ≥ 5
 *            Initial values: a(0)=1, a(1)=7, a(2)=36, a(3)=165, a(4)=747
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_size = 0;    /* number of elements stored           */
static idx_t  cache_cap  = 0;    /* capacity of the allocated array      */

/* Ensure that the array can hold at least "needed" elements (indexes 0 … needed-1). */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= needed)
        return;

    idx_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
    while (new_cap <= needed)
        new_cap <<= 1;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t*)malloc(sizeof(seq_t) * new_cap);
#endif

    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the nth term of A292486. */
seq_t a(idx_t n)
{
    /* Lazy initialization of the first five terms. */
    if (cache_size == 0) {
        ensure_capacity(5);
        cache[0] = 1;
        cache[1] = 7;
        cache[2] = 36;
        cache[3] = 165;
        cache[4] = 747;
        cache_size = 5;
    }

    if (n < cache_size)
        return cache[n];

    /* Grow the cache so that it can contain indices 0 … n. */
    ensure_capacity(n + 1);

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = 5 * cache[i - 1] - 4 * cache[i - 2]
                 + 9 * cache[i - 3] + 3 * cache[i - 4];
    }
    cache_size = n + 1;
    return cache[n];
}
