/*
 * A394762  Minimum number of draws without replacement from an urn with n
 *          white and n black balls to ensure the probability of drawing at
 *          least one ball of each color is >= n/(n+1).
 * https://oeis.org/A394762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min { k >= 1 : 2 * binomial(n, k) / binomial(2*n, k) <=
 *            1/(n+1) }
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 38, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Pre‑compute constants */
    idx_t two_n   = 2 * n;
    seq_t np1 = (seq_t)n + 1;            /* n+1 as seq_t */

    for (idx_t k = 1; ; ++k) {
        /* binomial(n,k) and binomial(2*n,k) are computed using the
           provided helper which supports arbitrary precision.     */
        seq_t bnk  = binomial(n, k);            /* C(n,k)   */
        seq_t left = bnk * (seq_t)2 * np1;      /* 2*C(n,k)*(n+1) */
        seq_t right = binomial(two_n, k);       /* C(2n,k)  */

        if (left <= right)          /* 2*C(n,k)*(n+1) <= C(2n,k)  */
            return (seq_t)k;        /* minimal k satisfying the condition */
    }
}
