/*
 * A352468  a(0) = 1; a(n) = Sum_{k=1..n} binomial(2*n,2*k)^3 * a(n-k).
 * https://oeis.org/A352468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=1..n} binomial(2*n,2*k)^3 * a(n-k) (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache     = NULL;   /* buffer for a(0)..a(n) */
    static size_t  cap      = 0;      /* allocated size */
    static idx_t   next     = 0;      /* first index yet to compute  (next ≥ 1) */

    /* initialise for a(0) */
    if (cap == 0) {
        cap   = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0; /* out of memory, caller must handle */
        cache[0] = 1;
        next     = 1;         /* only a(0) computed so far */
    }

    /* grow cache if required */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* out of memory */
        cache = tmp;
        /* initialise new slots to zero – not strictly needed but harmless */
        for (size_t i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = next; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t bin  = binomial(2 * i, 2 * k);      /* binomial(2i,2k)   */
            seq_t term = bin;
            term *= bin;  /* bin^2 */
            term *= bin;  /* bin^3 */
            term *= cache[(size_t)(i - k)];
            sum   += term;
        }
        cache[(size_t)i] = sum;
    }
    next = n + 1; /* all indices up to n are now computed */

    return cache[(size_t)n];
}
