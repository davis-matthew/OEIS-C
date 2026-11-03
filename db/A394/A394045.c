/*
 * A394045  a(0) = 0, a(n) = Sum_{k=1..n} binomial(n+1, k+1)*A053507(k) for n
 *          > 0.
 * https://oeis.org/A394045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} binomial(n+1, k+1) * ((k-1)*(k-2)/2) *
 *            k^(k-3)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (where binomial(n, k) is the ordinary binomial coefficient) */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* The first three terms are zero because (k-1)*(k-2) = 0 for k = 1,2 */
    if (n < 3)
        return 0;

    seq_t sum = 0;

    /* Start with binomial(n+1, 2) for k = 1 */
    seq_t binom = ((seq_t)(n + 1) * n) / 2;

    for (idx_t k = 1; k <= n; ++k)
    {
        if (k >= 3)
        {
            /* (k-1)*(k-2)/2 is always integer */
            seq_t factor = ((seq_t)(k - 1) * (k - 2)) / 2;

            /* k^(k-3) where the exponent is non‑negative for k ≥ 3 */
            seq_t powk = ipow_idx((seq_t)k, (idx_t)(k - 3));

            /* term = binom * factor * k^(k-3) */
            seq_t term = binom * factor;
            term = term * powk;

            sum += term;
        }

        /* Update binom to C(n+1, k+2) for the next iteration,
           if any.  The recurrence is exact: C(N, r+1) = C(N, r) * (N-r)/(r+1) */
        if (k < n)
            binom = binom * (n - k) / (k + 2);
    }

    return sum;
}
