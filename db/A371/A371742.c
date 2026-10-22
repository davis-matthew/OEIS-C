/*
 * A371742  a(n) = Sum_{k=0..floor(n/2)} binomial(3*n-k,n-2*k).
 * https://oeis.org/A371742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*n - k, n - 2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = n / 2;

    for (idx_t k = 0; k <= kmax; ++k)
    {
        sum += binomial(3 * n - k, n - 2 * k);
    }

    return sum;
}
