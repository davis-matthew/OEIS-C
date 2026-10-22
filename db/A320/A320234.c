/*
 * A320234  Expansion of Product_{k=1..8} theta_3(q^k), where theta_3() is the
 *          Jacobi theta function.
 * https://oeis.org/A320234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = coefficient of q^n in ∏_{k=1..8} θ₃(q^k), * equivalently
 *            the number of integer solutions to * a₁² + 2·a₂² + … + 8·a₈² = n
 * Range:     u8 n <= 14, u16 n <= 92, u32 n <= 3668, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;    /* a(0) = 1 */

    /* dp[t] holds the number of representations for t using the
       already processed factors.  Allocate using the utility to
       ensure proper initialization for arbitrary‑precision types. */
    seq_t *dp = alloc_seq_array((size_t)(n + 1));
    if (!dp) return (seq_t)0;
    dp[0] = (seq_t)1;   /* other entries are already zero */

    for (idx_t k = 1; k <= 8; ++k)
    {
        seq_t *newdp = alloc_seq_array((size_t)(n + 1));
        if (!newdp) { free_seq_array(dp); return (seq_t)0; }

        /* Convolve dp with the coefficients of θ₃(q^k).  The
           coefficient of q^{k·m²} is 1 for m = 0 and 2 for |m| > 0. */
        for (idx_t m = 0; ; ++m)
        {
            __int128 tmp = (__int128)k * m * m;
            if (tmp > n) break;
            idx_t diff = (idx_t)tmp;
            seq_t ways = (m == 0) ? (seq_t)1 : (seq_t)2;

            if (diff == 0)
            {
                for (idx_t t = 0; t <= n; ++t)
                    newdp[t] += dp[t] * ways;
            }
            else
            {
                for (idx_t t = 0; t + diff <= n; ++t)
                    newdp[t + diff] += dp[t] * ways;
            }
        }

        free_seq_array(dp);
        dp = newdp;
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
