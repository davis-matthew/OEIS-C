/*
 * A383572  Expansion of 1/sqrt((1-x^4)^2 - 4*x^5).
 * https://oeis.org/A383572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(n-3*k,k) * binomial(k,
 *            n-4*k)
 * Range:     u8 n <= 19, u16 n <= 36, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 4;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t n1 = n - 3 * k;
        seq_t term1 = binomial(n1, k);
        idx_t m2 = n - 4 * k;
        if (m2 > k)
            continue;
        seq_t term2 = binomial(k, m2);
        sum += term1 * term2;
    }
    return sum;
}
