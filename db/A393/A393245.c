/*
 * A393245  a(n) = Sum_{k=0..floor(n/4)} binomial(2*k,k) * binomial(n,4*k).
 * https://oeis.org/A393245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(2*k,k) * binomial(n,4*k)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 28, u64 n <= 53, u128 n <= 104,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 4; ++k)
    {
        seq_t c1 = binomial(2 * k, k);
        seq_t c2 = binomial(n, 4 * k);
        sum += c1 * c2;
    }
    return sum;
}
