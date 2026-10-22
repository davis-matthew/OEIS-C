/*
 * A308806  Expansion of 1 / Sum_{k>=0} (-x)^(k*(3*k - 1)/2).
 * https://oeis.org/A308806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 44, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients of the series 1 / Sum_{k>=0} (-x)^(k*(3*k-1)/2)
   Recurrence: a(0) = 1;
   For n > 0,
       a[n] = Σ_{k>=1, pentank ≤ n} (-1)^(pentank+1) * a[n - pentank],
   where pentank = k*(3*k-1)/2. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoized values */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t  cache_next = 0;        /* next index to compute (== cache_cap as int) */

seq_t a(idx_t n) {
    /* Ensure cache can hold indices up to n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;                     /* grow exponentially */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();              /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize base case if this is the first call */
    if (cache_next == 0) {
        cache[0] = (seq_t)1;
        cache_next = 1;
    }

    /* Compute values from cache_next up to n */
    for (idx_t i = cache_next; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1;; ++k) {
            idx_t pent = k * (3 * k - 1) / 2;   /* pentagonal number */
            if (pent > i)
                break;
            seq_t term = cache[i - pent];
            if (pent & 1)        /* odd pentagon -> +1 */
                sum += term;
            else                 /* even pentagon -> -1 */
                sum -= term;
        }
        cache[i] = sum;
    }
    cache_next = n + 1;

    return cache[n];
}
