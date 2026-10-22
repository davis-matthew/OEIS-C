/*
 * A364510  a(n) = binomial(4*n, n)^2.
 * https://oeis.org/A364510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = binomial(4*n, n)^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute binomial(4*n, n) and square it */
    seq_t bin = binomial((idx_t)(4 * n), n);
    return bin * bin;
}
