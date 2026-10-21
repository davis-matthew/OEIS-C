/*
 * A261801  Number of 9-compositions of n: matrices with 9 rows of nonnegative
 *          integers with positive column sums and total element sum n.
 * https://oeis.org/A261801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k>=0} (1/2)^(k+1) * binomial(n-1+9*k, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence satisfies the linear recurrence derived from the
 * generating function: for n >= 0
 * a(n) = 18*a(n-1) - 72*a(n-2) + 168*a(n-3) - 252*a(n-4)
 *        + 252*a(n-5) - 168*a(n-6) + 72*a(n-7) - 18*a(n-8) + 2*a(n-9)
 *        + c(n) where c(n) = (-1)^n * binomial(9,n) for n <= 9, otherwise 0.
 * This implementation memoises the terms to allow arbitrary order
 * of access up to a very large n.                                           */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;      /* Cache of computed terms a(0).. */
static size_t cache_cap = 0;     /* Number of terms stored (cache[0..cache_cap-1]) */

seq_t a(idx_t n)
{
    /* Grow the cache if necessary */
    size_t nsize = (size_t)n;
    if (nsize >= cache_cap) {
        size_t new_cap = nsize + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* Out of memory */
        cache = tmp;
        /* Compute missing terms */
        for (size_t i = cache_cap; i <= nsize; ++i) {
            seq_t val = 0;

            /* Positive coefficient terms */
            if (i >= 1) val += cache[i-1] * (seq_t)18;
            if (i >= 3) val += cache[i-3] * (seq_t)168;
            if (i >= 5) val += cache[i-5] * (seq_t)252;
            if (i >= 7) val += cache[i-7] * (seq_t)72;
            if (i >= 9) val += cache[i-9] * (seq_t)2;

            /* Negative coefficient terms */
            if (i >= 2) val -= cache[i-2] * (seq_t)72;
            if (i >= 4) val -= cache[i-4] * (seq_t)252;
            if (i >= 6) val -= cache[i-6] * (seq_t)168;
            if (i >= 8) val -= cache[i-8] * (seq_t)18;

            /* Add the c(n) term when n <= 9 */
            if (i <= 9) {
                seq_t bin = binomial(9, (idx_t)i);
                if ((i & 1ULL) == 0)   /* even i => positive */
                    val += bin;
                else                   /* odd i  => negative */
                    val -= bin;
            }

            cache[i] = val;
        }
        cache_cap = new_cap;
    }
    return cache[nsize];
}
