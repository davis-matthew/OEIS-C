/*
 * A317297  a(n) = (n - 1)*(4*n^2 - 8*n + 5).
 * https://oeis.org/A317297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1)*(4*n^2 - 8*n + 5)
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* data for n >= 1 only */

    seq_t nn = (seq_t)n;                /* cast index to seq_t for safe math */
    seq_t part = 4 * nn * nn - 8 * nn + 5;
    return (nn - 1) * part;
}
