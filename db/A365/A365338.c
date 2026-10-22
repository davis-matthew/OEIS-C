/*
 * A365338  Expansion of e.g.f. exp( Sum_{k>=0} x^(3*k+2) / (3*k+2)! ).
 * https://oeis.org/A365338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊(n-2)/3⌋} binomial(n-1,3*k+1) * a(n-3*k-2)
 *            Initial values: a(0)=1, a(1)=0, a(2)=1
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t   last   = (idx_t)-1;   /* no values yet */

    /* Initialise cache if this is the first call */
    if (cache == NULL) {
        /* Guarantee at least two entries for a(0) and a(1) */
        cap = n < 2 ? 2 : (size_t)(n + 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */

        cache[0] = (seq_t)1;                /* a(0) */
        cache[1] = (seq_t)0;                /* a(1) */
        last = 1;
    }

    /* Return stored value if already computed */
    if (n <= last) return cache[n];

    /* Grow the cache if required */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap : 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t m = last + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0;; ++k) {
            idx_t offset = 3 * k + 2;            /* 3k+2 */
            if (offset > m) break;
            /* binomial(m-1, 3k+1) -> binomial(m-1, offset-1) */
            seq_t bin  = binomial(m - 1, offset - 1);
            seq_t term = bin * cache[m - offset];
            sum += term;
        }
        cache[m] = sum;
    }

    last = n;
    return cache[n];
}
