/*
 * A337951  a(n) = 4^(n*(n-1)/2) + 4^(n*(n+1)/2) for n > 0, with a(0) = 1.
 * https://oeis.org/A337951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(n*(n-1)/2) + 4^(n*(n+1)/2) for n > 0, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* exponent for the first term: n*(n-1)/2 */
    idx_t e1 = n * (n - 1) / 2;

    /* exponent for the second term: n*(n+1)/2 */
    idx_t e2 = n * (n + 1) / 2;

    seq_t p1 = seq_pow((seq_t)4, e1);
    seq_t p2 = seq_pow((seq_t)4, e2);

    return p1 + p2;
}
