/*
 * A361743  Central circular Delannoy numbers: a(n) is the number of Delannoy
 *          loops on an n X n toroidal grid.
 * https://oeis.org/A361743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n} binomial(n,k)^2 * k * 2^k for n >= 1.
 * For n = 0, a(0) = 1.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum  = 0;
    seq_t binom = (seq_t)1;   /* binomial(n,0) */
    seq_t pow2  = (seq_t)1;   /* 2^0 */

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k > 0) {
            /* binom = binom(n, k) = binom(n, k-1) * (n - k + 1) / k */
            seq_t numerator = binom * (seq_t)(n - (k - 1));
            seq_t denom     = (seq_t)k;
            binom = numerator / denom;          /* exact division */
            pow2 = mul_small(pow2, 2);          /* multiply by 2 */
        }

        if (k == 0)      /* term is zero for k = 0 */
            continue;

        seq_t term = binom;
        term = term * binom;          /* binom^2 */
        term = term * (seq_t)k;       /* multiply by k */
        term = term * pow2;           /* multiply by 2^k */
        sum += term;
    }

    return sum;
}
