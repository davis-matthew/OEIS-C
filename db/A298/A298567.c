/*
 * A298567  a(n) = Sum_{k=0..2*n/3} C(n-k,2*k-n)^2.
 * https://oeis.org/A298567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊2*n/3⌋} C(n-k, 2*k-n)^2
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k_start = (n + 1) / 2;   /* ceil(n/2) */
    idx_t k_end   = (2 * n) / 3;   /* floor(2*n/3) */

    for (idx_t k = k_start; k <= k_end; ++k)
    {
        idx_t upper = n - k;
        idx_t lower = 2 * k - n;
        seq_t c = binomial(upper, lower);
        seq_t sq = c * c;
        sum += sq;
    }
    return sum;
}
