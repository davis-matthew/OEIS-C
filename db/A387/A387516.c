/*
 * A387516  a(n) = Sum_{k=0..floor(n/3)} 2^(n-3*k) * binomial(k,n-3*k)^2.
 * https://oeis.org/A387516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^(n-3*k) * binomial(k, n-3*k)^2
 * Range:     u8 n <= 16, u16 n <= 27, u32 n <= 51, u64 n <= 96, u128 n <= 186,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k * 3 <= n; ++k)
    {
        idx_t m = n - 3 * k;
        seq_t bin = binomial(k, m);
        seq_t term = pow2(m);
        term = term * bin * bin;
        sum = sum + term;
    }
    return sum;
}
