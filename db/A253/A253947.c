/*
 * A253947  a(n) = 6*binomial(n+1,7).
 * https://oeis.org/A253947
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*binomial(n+1,7)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 63, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* compute 6 * C(n+1,7) */
    return (seq_t)6 * binomial(n + 1, 7);
}
