/*
 * A276124  a(0) = a(1) = a(2) = a(3) = 1; for n > 3, a(n) =
 *          (a(n-1)^2+a(n-2)^2+a(n-3)^2+a(n-1)*a(n-2)*a(n-3))/a(n-4).
 * https://oeis.org/A276124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A276124:
 * a(0) = a(1) = a(2) = a(3) = 1;
 * a(n) = (a(n‑1)^2 + a(n‑2)^2 + a(n‑3)^2 + a(n‑1)*a(n‑2)*a(n‑3)) / a(n‑4)    for n > 3
 * (recurrence is exact; uses only integer arithmetic)
 */

#include <stdlib.h>

/* Private cache for memoised values.
 * cache_cap  – allocated size (>= cache_sz)
 * cache_sz   – number of values that have been computed and stored
 */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_sz  = 0;

/* Ensure that the cache can hold at least 'needed' elements (indices 0 .. needed‑1). */
static void ensure_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;

    /* realloc returns void*; cast is required in C++ */
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) abort();            /* out of memory – terminate */
    cache      = tmp;
    cache_cap  = needed;
}

/* Return the n-th term of A276124.  Works for any call order and memoises results. */
seq_t a(idx_t n)
{
    /* Return already cached value */
    if (n < cache_sz)
        return cache[n];

    /* Initialise base values if they are not present yet */
    if (cache_sz < 4) {
        ensure_cache(4);    /* at least space for indices 0..3 */
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)1;
        cache_sz = 4;
    }

    /* Expand cache to accommodate the requested index */
    if (n + 1 > cache_cap)
        ensure_cache(n + 1);

    /* Compute missing terms up to n inclusive */
    for (idx_t i = cache_sz; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];

        seq_t sq1   = a1 * a1;
        seq_t sq2   = a2 * a2;
        seq_t sq3   = a3 * a3;
        seq_t prod  = a1 * a2;
        prod        = prod * a3;

        seq_t sum   = sq1 + sq2 + sq3 + prod;
        cache[i]    = sum / a4;     /* exact division per definition */
    }

    /* Update the count of stored values */
    cache_sz = n + 1;
    return cache[n];
}
