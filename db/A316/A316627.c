/*
 * A316627  a(1)=2, a(2)=3; a(n) = a(n+1-a(n-1))+a(n-a(n-2)) for n > 2.
 * https://oeis.org/A316627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(2) = 3; a(n) = a(n+1-a(n-1)) + a(n-a(n-2)) for n > 2
 * Range:     u8 n <= 494, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;  /* number of computed indices (at least 0) */

/* create or extend cache up to n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    size_t new_cap = (size_t)n + 1;          /* indices 0 … n (0 unused) */
    seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_mem;

    for (idx_t i = cache_cap + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)2;
        else if (i == 2)
            cache[i] = (seq_t)3;
        else {
            idx_t j1 = i + 1 - (idx_t)cache[i - 1];
            idx_t j2 = i - (idx_t)cache[i - 2];
            cache[i] = cache[j1] + cache[j2];
        }
    }
    cache_cap = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, return 0 to avoid UB */

    ensure_cache(n);
    return cache[n];
}
