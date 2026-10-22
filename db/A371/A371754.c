/*
 * A371754  a(n) = Sum_{k=0..floor(n/3)} binomial(3*n-2*k,n-3*k).
 * https://oeis.org/A371754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(3*n-2*k, n-3*k)
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
    idx_t kmax = n / 3;
    for (idx_t k = 0; k <= kmax; ++k)
    {
        sum += binomial(3 * n - 2 * k, n - 3 * k);
    }
    return sum;
}
