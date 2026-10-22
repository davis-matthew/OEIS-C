/*
 * A357537  a(n) = 2*A080635(n) if n > 0. a(0) = 1.
 * https://oeis.org/A357537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = (1/2) * Σ_{k=0}^{n-1} binom(n-1,k) *
 *            a(k) * a(n-1-k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, calloc, realloc, free */
#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* Cache of already computed values (supports non‑monotone calls)   */
    static seq_t *cache      = NULL;
    static idx_t  cache_size = 0;   /* number of entries actually used */

    /* If we already have the value, return it immediately.          */
    if (n < cache_size)
        return cache[(size_t)n];

    /* Grow the cache to the required size.                           */
    if (cache == NULL) {
        cache = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    } else if ((size_t)(n + 1) > cache_size) {
        cache = (seq_t *)realloc(cache,
                                 (size_t)(n + 1) * sizeof(seq_t));
    }

    /* Compute missing values from the current cache_size up to n.   */
    for (idx_t k = cache_size; k <= n; ++k) {
        if (k == 0) {                     /* a(0) = 1 */
            cache[(size_t)k] = (seq_t)1;
        } else if (k == 1) {               /* a(1) = 2 */
            cache[(size_t)k] = (seq_t)2;
        } else {
            /* Compute a(k) using the recurrence
               a(k) = (1/2) * Σ_{i=0}^{k-1} C(k-1,i) * a(i) * a(k-1-i)  */
            idx_t limit  = k - 1;          /* maximum i */
            idx_t half   = limit / 2;      /* for symmetry */
            seq_t sum = 0;
            seq_t bin = 1;                 /* C(limit,0) */

            for (idx_t i = 0; i <= half; ++i) {
                idx_t j = limit - i;
                seq_t term = bin * cache[(size_t)i];
                term = term * cache[(size_t)j];

                if (i < j)
                    sum += term * (seq_t)2;   /* double the symmetric term */
                else
                    sum += term;              /* central term appears once */

                /* Update binomial coefficient for next i */
                if (i < limit) {
                    seq_t num = (seq_t)(limit - i);
                    seq_t den = (seq_t)(i + 1);
                    bin = bin * num / den;    /* exact division */
                }
            }

            cache[(size_t)k] = sum / (seq_t)2;   /* division by 2 is exact */
        }
    }

    cache_size = n + 1;   /* we have filled up to n */
    return cache[(size_t)n];
}
