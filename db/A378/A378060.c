/*
 * A378060  a(n) = binomial(n, floor((n-1)/2))^2.
 * https://oeis.org/A378060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = binomial(n, floor((n-1)/2))^2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) is defined as 0 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* floor((n-1)/2) = (n-1)/2 with integer division */
    idx_t k = (n - 1) / 2;

    seq_t val = binomial(n, k);
    return val * val;
}
