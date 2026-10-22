/*
 * A371778  a(n) = Sum_{k=0..floor(n/3)} binomial(3*n+2,n-3*k).
 * https://oeis.org/A371778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(3*n+2, n-3*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t m = 3 * n + 2;
    idx_t k_max = n / 3;
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= k_max; ++k)
    {
        idx_t r = n - 3 * k;
        sum += binom(m, r);
    }
    return sum;
}
