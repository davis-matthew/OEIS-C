/*
 * A358496  a(n) = Sum_{k=0..n} binomial(binomial(n, k), k).
 * https://oeis.org/A358496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(binomial(n, k), k)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Computes C(N, k) as a seq_t; assumes N >= k. */
static seq_t binom_big(seq_t N, idx_t k)
{
    if (k == 0) return (seq_t)1;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* multiplication then division – exact because of binomial properties */
        res = res * (N - k + (seq_t)i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)1;           /* contribution from k = 0 */
    seq_t binom_nk = (seq_t)1;      /* C(n, 0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update binom(n, k) iteratively to avoid O(n^2) inner work */
        binom_nk = binom_nk * (seq_t)(n - k + 1) / (seq_t)k;

        if (binom_nk >= (seq_t)k)
        {
            sum += binom_big(binom_nk, k);
        }
    }
    return sum;
}
