/*
 * A350469  a(n) = hypergeom([1/2 - n/2, -n/2], [-n], -72).
 * https://oeis.org/A350469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A350469
   Recurrence: a(0)=1, a(1)=1, a(n)=a(n-1)+18*a(n-2) for n>=2
   Generating function: 1/(1-x-18*x^2)
   The function below memoises the terms in a static array so
   subsequent calls are O(1) after the first evaluation.  All
   operations are performed with seq_t arithmetic only. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of the array */
    static idx_t cache_len = 0;    /* number of elements currently stored */

    /* initialise cache on first use */
    if (!cache) {
        /* at least two slots are required for the base cases */
        cache_cap = (n < 1u) ? 2 : (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out of memory: return 0 */
        cache[0] = (seq_t)1;
        if (cache_cap > 1) cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* grow the cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2; /* overshoot to reduce reallocs */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;      /* out of memory: return 0 */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    while (cache_len <= n) {
        seq_t prod = cache[cache_len - 2] * (seq_t)18;
        cache[cache_len] = cache[cache_len - 1] + prod;
        ++cache_len;
    }

    return cache[n];
}
