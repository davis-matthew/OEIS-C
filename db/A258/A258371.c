/*
 * A258371  Triangle read by rows: T(n,k) is number of ways of arranging n
 *          indistinguishable points on an n X n square grid such that k rows
 *          contain at least one point.
 * https://oeis.org/A258371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r and k are obtained from the index n by
 *            solving n = r(r-1)/2 + k (1 ≤ k ≤ r). T(r,k) = binomial(r,k) *
 *            Σ_{e=0..k} (-1)^e * binomial(k,e) * binomial((k-e)*r, r)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 39, u64 n <= 97, u128 n <= 264,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"

seq_t a(idx_t idx)
{
    /* determine r (row) and k (column) from idx */
    idx_t r = (idx_t)((sqrt((double)8 * idx + 1.0) - 1.0) / 2.0);
    while (r * (r + 1) / 2 < idx) r++;          /* ensure r satisfies the inequality */
    idx_t k = idx - (r - 1) * r / 2;            /* k in 1..r */

    /* compute S = Σ_{e=0..k} (-1)^e * C(k,e) * C((k-e)*r, r) */
    seq_t S = 0;
    for (idx_t e = 0; e <= k; ++e)
    {
        seq_t term = binom(k, e) * binom((k - e) * r, r);
        if (e & 1U)
            S -= term;
        else
            S += term;
    }

    /* final value */
    return binom(r, k) * S;
}
