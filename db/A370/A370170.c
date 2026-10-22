/*
 * A370170  Coefficient of x^n in the expansion of (1+x+x^2)^(3*n).
 * https://oeis.org/A370170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binom(3*n,k)*binom(3*n-k,n-2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t maxk = n >> 1;                /* floor(n/2) */
    idx_t three_n = n * 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t c1 = binom(three_n, k);
        seq_t c2 = binom(three_n - k, n - (k << 1));
        result += c1 * c2;
    }
    return result;
}
