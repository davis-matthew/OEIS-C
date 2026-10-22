/*
 * A342482  a(n) = n*(2^(n-1) - n - 1).
 * https://oeis.org/A342482
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2^(n-1) - n - 1)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* compute 2^(n-1) */
    seq_t power = pow2(n - 1);

    /* subtract n and 1 */
    seq_t term = power - (seq_t)n - 1;

    /* multiply by n */
    return (seq_t)n * term;
}
