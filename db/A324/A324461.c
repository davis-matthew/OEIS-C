/*
 * A324461  Number of simple graphs with n vertices and distinct rotations.
 * https://oeis.org/A324461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d)·2^( n·(n/d-1)/2 + n·⌊d/2⌋/d ) for n>0, a(0)
 *            = 1 where μ is the Möbius function
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   mobius, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mobius.h"   /* seq_t mobius(idx_t)      */
#include "../utils/pow2.h"     /* seq_t pow2(idx_t)        */

/* Compute a(n) for n >= 0.  The function is memoized to avoid repeated work
   if called many times, but memoization is optional since the computation is
   O(n). */
seq_t a(idx_t n)
{
    /* Special case for the offset. */
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;

    /* Iterate over all divisors d of n.  Since n <= 10 000 in the tests,
       a simple loop is fast enough. */
    for (idx_t d = (idx_t)1; d <= n; ++d) {
        if (n % d != (idx_t)0)
            continue;

        /* Möbius function value for divisor d.  The utility returns a
           seq_t, but it really holds -1, 0, or +1, so we convert to int. */
        int mu = (int)mobius(d);
        if (mu == 0)
            continue;

        /* Exponent in the formula.  All calculations fit in idx_t
           (64‑bit) for the problem size. */
        idx_t term1 = n * ((n / d) - (idx_t)1) / (idx_t)2;          /* n*(n/d-1)/2 */
        idx_t term2 = n * (d / (idx_t)2) / d;                       /* n*⌊d/2⌋/d   */
        idx_t exp   = term1 + term2;

        /* 2^exp as seq_t.  The pow2 utility returns a sequence type. */
        seq_t contrib = pow2(exp);

        /* Add or subtract depending on μ(d). */
        if (mu > 0)
            sum += contrib;
        else
            sum -= contrib;
    }

    return sum;
}
