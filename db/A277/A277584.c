/*
 * A277584  a(n) = binomial(3*n-1, n-1)^2.
 * https://oeis.org/A277584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n-1, n-1)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Computes A277584: a(n) = C(3*n-1, n-1)^2 with a(0)=0 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t b = binom(3 * n - 1, n - 1);
    return b * b;
}
