/*
 * A318595  Number of n-member subsets of [7*n] whose elements sum to a
 *          multiple of seven.
 * https://oeis.org/A318595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/7)(binom(7*n, n) + 6*binom(n, n/7) if 7|n else 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* multiplicative binomial computation in seq_t
   safe for indices up to idx_t limits and for seq_t width that can
   hold the final result. */
static seq_t binom_idx(idx_t N, idx_t K)
{
    if (K > N - K)
        K = N - K;
    seq_t result = 1;
    for (idx_t i = 1; i <= K; ++i)
    {
        seq_t num   = (seq_t)(N - K + i);
        seq_t denom = (seq_t)i;

        /* cancel gcd between numerator and denominator */
        seq_t g = gcd(num, denom);
        num /= g;
        denom /= g;

        /* cancel remaining gcd with current result */
        g = gcd(result, denom);
        result /= g;
        denom /= g;

        /* denom should be 1 now */
        result *= num;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t bin = binom_idx((idx_t)7 * n, n);

    seq_t small = 0;
    if (n % 7 == 0)
        small = binom_idx(n, n / 7);

    seq_t sum = bin + 6 * small;
    return sum / 7;
}
