/*
 * A276925  Number of ordered set partitions of [n] with at most five elements
 *          per block.
 * https://oeis.org/A276925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A276925: Number of ordered set partitions of [n] with at most five elements per block
 *
 * Recurrence (convenient for integer arithmetic):
 *   a(0) = 1
 *   a(n) = Σ_{i=1}^{min(5,n)} C(n,i) * a(n-i)      for n ≥ 1
 *
 * The recurrence follows from fixing the first block of size i (choosing i elements
 * from n; C(n,i) ways) and then partitioning the remaining n−i elements in any
 * allowed ordered way (a(n−i) ways).  The sum over i accounts for all possible
 * first‐block sizes (1 to 5).
 *
 * The function memoises results in a static cache so that each value is computed
 * only once, regardless of the order in which the function is called.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"      /* declares binomial(idx_t n, idx_t k) : seq_t */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap = 0;

    /* initialise cache on first call */
    if (cache_cap == 0)
    {
        cache_cap = 1;                         /* enough for n = 0 */
        cache    = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;    /* minimal error handling */
        cache[0] = (seq_t)1;                   /* a(0) = 1 */
    }

    /* expand cache if necessary */
    if ((size_t)(n + 1) > cache_cap)
    {
        size_t new_cap = (size_t)(n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;      /* minimal error handling */
        /* initialise new entries to zero */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if already computed, return cached value */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* compute values up to n (ascending order) */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i] != (seq_t)0)
            continue;          /* already computed */

        seq_t total = (seq_t)0;
        for (idx_t k = 1; k <= 5 && k <= i; ++k)
        {
            seq_t binom = binomial(i, k);        /* C(i,k) */
            seq_t term  = binom * cache[i - k];
            total += term;
        }
        cache[i] = total;
    }

    return cache[n];
}
