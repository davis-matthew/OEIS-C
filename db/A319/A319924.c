/*
 * A319924  a(n) = A143565(2n,n) for n > 0, a(0) = 1.
 * https://oeis.org/A319924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + n * binomial(2n, n) (a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t bin = binomial(2 * n, n);
    return (seq_t)1 + (seq_t)n * bin;
}
