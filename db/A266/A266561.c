/*
 * A266561  12-dimensional square numbers.
 * https://oeis.org/A266561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+11,11)*(n+6)/6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute C(n+11,11) first */
    seq_t c = binomial(n + 11, 11);
    /* Multiply by (n+6) and divide by 6 */
    return c * ((seq_t)(n + 6)) / 6;
}
