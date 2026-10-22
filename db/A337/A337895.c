/*
 * A337895  Number of oriented colorings of the tetrahedral facets (or
 *          vertices) of a regular 4-dimensional simplex using n or fewer
 *          colors.
 * https://oeis.org/A337895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4,5) + binomial(n,5)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses mul_small to avoid overflow on 64‑bit builds. */

#include "../utils/mul_small.h"

static seq_t binom5(idx_t m)
{
    if (m < 5) return 0;
    seq_t res = 1;
    /* Compute product m*(m-1)*(m-2)*(m-3)*(m-4) safely. */
    for (idx_t k = 0; k < 5; ++k)
    {
        unsigned int factor = (unsigned int)(m - k);
        res = mul_small(res, factor);
    }
    /* Divide by 5! = 120. */
    res /= (seq_t)120;
    return res;
}

seq_t a(idx_t n)
{
    seq_t c1 = binom5(n + 4);  /* C(n+4,5) */
    seq_t c2 = binom5(n);      /* C(n,5)   */
    return c1 + c2;
}
