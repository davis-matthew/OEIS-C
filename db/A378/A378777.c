/*
 * A378777  a(n) = n^2 * binomial(3*n, n).
 * https://oeis.org/A378777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * binomial(3*n, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t nsq = (seq_t)n * (seq_t)n;
    seq_t bi  = binomial((idx_t)(3 * n), n);
    return nsq * bi;
}
