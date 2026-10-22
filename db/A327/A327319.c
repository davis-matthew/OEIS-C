/*
 * A327319  a(n) = binomial(n, 2) + 6*binomial(n, 4).
 * https://oeis.org/A327319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n, 2) + 6 * binomial(n, 4)
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 363, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t r2 = binom(n, 2);   /* binomial(n,2) */
    seq_t r4 = binom(n, 4);   /* binomial(n,4) */
    return r2 + 6 * r4;
}
