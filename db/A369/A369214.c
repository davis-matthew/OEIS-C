/*
 * A369214  Expansion of (1/x) * Series_Reversion( x * ((1-x)^2-x^3) ).
 * https://oeis.org/A369214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(n+k,k) *
 *            binomial(3*n-k+1,n-3*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) = 1 */
        return (seq_t)1;

    idx_t maxk = n / 3;
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* first binomial: C(n+k, k) */
        idx_t a1 = n + k;
        idx_t b1 = k;
        seq_t t1 = binomial(a1, b1);

        /* second binomial: C(3*n - k + 1, n - 3*k) */
        idx_t a2 = 3 * n - k + 1;
        idx_t b2 = n - 3 * k;
        seq_t t2 = binomial(a2, b2);

        /* multiply and add to sum */
        seq_t prod = t1 * t2;
        sum += prod;
    }

    seq_t denom = n + 1;
    return sum / denom;
}
