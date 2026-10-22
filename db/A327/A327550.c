/*
 * A327550  Number of compositions of partitions of 2n with exactly n
 *          compositions.
 * https://oeis.org/A327550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) * A000079(n) (partition(n) · 2^n)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 100,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t   */
#include <stdlib.h>   /* malloc, free */
#include "../utils/seq_mul.h"   /* seq_mul */
#include "../utils/pow2.h"      /* pow2 */

/* Cache of partition numbers p(i).  cache[i] = p(i). */
static seq_t *cache = NULL;      /* pointer to the array */
static size_t cache_cap = 0;      /* how many entries can be stored */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_partitions(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;                           /* nothing to do */

    seq_t *new_cache = (seq_t *)malloc(needed * sizeof(seq_t));
    if (!new_cache)                            /* allocation failed */
        return;

    /* copy old values */
    for (size_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];

    /* initialise new slots to zero */
    for (size_t i = cache_cap; i < needed; ++i)
        new_cache[i] = 0;

    /* free old buffer */
    free(cache);

    cache = new_cache;
    cache_cap = needed;

    /* p(0) = 1  – already zeroed, set explicitly if still zero */
    if (cache_cap > 0 && cache[0] == 0)
        cache[0] = 1;
}

/* Compute partition numbers p(i) for all i ≤ n using Euler’s pentagonal
   recurrence.  Only values that are not already cached are recomputed. */
static void compute_partitions(idx_t n)
{
    size_t prev_cap = cache_cap;   /* capacity before potential growth */
    ensure_partitions(n);

    /* After ensure_partitions, cache[0] is guaranteed to be 1. */
    if (prev_cap == 0 && n >= 0) {
        cache[0] = 1;
        prev_cap = 1;
    }

    if (prev_cap > n)
        return;                     /* all needed values already known */

    for (idx_t i = (idx_t)prev_cap; i <= n; ++i) {
        if (i == 0) { cache[0] = 1; continue; }

        seq_t sum = 0;
        for (idx_t m = 1; ; ++m) {
            idx_t g1 = m * (3 * m - 1) / 2;
            if (i < g1)
                break;
            seq_t term = cache[i - g1];
            sum += (m & 1) ? term : -term;

            idx_t g2 = m * (3 * m + 1) / 2;
            if (i < g2)
                continue;
            term = cache[i - g2];
            sum += (m & 1) ? term : -term;
        }
        cache[i] = sum;
    }
}

/* Public entry point.  Computes a(n) = p(n) * 2^n. */
seq_t a(idx_t n)
{
    compute_partitions(n);
    seq_t pow2_n = pow2(n);                /* 2^n */
    return seq_mul(cache[n], pow2_n);      /* p(n) * 2^n */
}
