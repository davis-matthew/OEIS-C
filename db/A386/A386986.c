/*
 * A386986  a(n) = Sum_{k=0..n} (k+1) * 8^k * binomial(2*n+2,n-k).
 * https://oeis.org/A386986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * 8^k * binomial(2*n+2, n-k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t bin   = binomial(2*n + 2, n);   /* C(2n+2, n) */
    seq_t power = 1;                      /* 8^k, k starts at 0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = (k+1) * 8^k * C(2n+2, n-k) */
        seq_t term = bin * power;
        term = term * ((seq_t)(k + 1));
        sum += term;

        if (k == n) break;                /* last term; no need to update bin */

        /* Update binomial for next k:
           C(2n+2, n-k-1) = bin * (n-k) / (n+k+3) */
        bin = (bin * ((seq_t)(n - k))) / ((seq_t)(n + k + 3));

        power *= 8;                       /* advance 8^k -> 8^(k+1) */
    }
    return sum;
}
