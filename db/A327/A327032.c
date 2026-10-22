/*
 * A327032  a(n) = T(n, 4) with T(n, k) = Sum_{d|k} phi(d)*binomial(n - 1 +
 *          k/d, k/d).
 * https://oeis.org/A327032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3,4) + binomial(n+1,2) + 2*n
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t term1 = binomial(n + 3, 4);
    seq_t term2 = binomial(n + 1, 2);
    seq_t term3 = (seq_t)2 * n;
    return term1 + term2 + term3;
}
