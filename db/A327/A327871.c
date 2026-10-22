/*
 * A327871  Number of self-avoiding planar walks starting at (0,0), ending at
 *          (n,n), remaining in the first quadrant and using steps (0,1),
 *          (-1,1), and (1,-1) with the restriction that (-1,1) and (1,-1) are
 *          always immediately followed by (0,1).
 * https://oeis.org/A327871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = Sum_{j=0..floor((n-1)/2)} binom(2*n-1, 2*j+n) * *
 *            (binom(2*j+n, j)-binom(2*j+n, j-1))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"

/* Cache for computed values */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;

/* Compute a(n) as defined above.  Results are cached for future calls. */
seq_t a(idx_t n)
{
    /* Grow cache to accommodate n */
    if (n >= a_cache_cap) {
        size_t old_cap = a_cache_cap;
        a_cache_cap = n + 1;
        a_cache = (seq_t *)realloc(a_cache, a_cache_cap * sizeof(seq_t));
        if (!a_cache) abort();
        for (size_t i = old_cap; i < a_cache_cap; ++i)
            a_cache[i] = 0;
    }

    /* Return cached value if already computed */
    if (a_cache[n] != 0)
        return a_cache[n];

    /* Compute a(n) */
    seq_t result = 0;
    if (n == 0) {
        result = 1;
    } else {
        idx_t m      = 2 * n - 1;
        idx_t max_j  = (n - 1) / 2;
        for (idx_t j = 0; j <= max_j; ++j) {
            idx_t k = 2 * j + n;
            seq_t bin1 = binom(m, k);
            seq_t bin2 = binom(k, j);
            seq_t bin3 = (j >= 1) ? binom(k, j - 1) : 0;
            seq_t term = bin1 * (bin2 - bin3);
            result += term;
        }
    }

    a_cache[n] = result;
    return result;
}
