/*
 * A337282  Partial sums of A337281.
 * https://oeis.org/A337282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 32, u64 n <= 67, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A337282: Partial sums of A337281.
   Recurrence (for n>6):
      a(n) = 3*a(n-1) – a(n-2) – a(n-3) – 3*a(n-4) + a(n-5) + a(n-6) + a(n-7)
   Initial values:
      a(0)=0, a(1)=0, a(2)=2, a(3)=5, a(4)=13,
      a(5)=33, a(6)=75
   The function is memoised so that any order of calls produces
   the correct result for all n ≥ 0. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache holds all computed terms, cache_cap is the allocated size */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* initialise the first seven terms once */
    if (cache_cap == 0) {
        cache_cap = 7;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)33;
        cache[6] = (seq_t)75;
    }

    /* if the requested term is already known, return it directly */
    if (n < cache_cap)
        return cache[n];

    /* remember the size of the cache before any potential reallocation */
    idx_t old_cap = cache_cap;

    /* ensure the cache is large enough for indices up to n */
    if (n + 1 > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = n + 1;
    }

    /* compute any missing values from old_cap up to n */
    for (idx_t i = old_cap; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];
        seq_t a7 = cache[i - 7];
        seq_t val = 3 * a1 - a2 - a3 - 3 * a4 + a5 + a6 + a7;
        cache[i] = val;
    }

    return cache[n];
}
