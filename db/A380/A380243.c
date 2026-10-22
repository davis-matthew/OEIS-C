/*
 * A380243  Number of rooted 2n-regular planar maps with 3 vertices.
 * https://oeis.org/A380243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*binomial(2*n-1,n)^3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute C(2*n-1, n) */
    seq_t bin = binomial(2 * n - 1, n);

    /* Compute bin^3 */
    seq_t cube = bin;
    cube *= bin;
    cube *= bin;

    /* Multiply by n and return */
    return (seq_t)n * cube;
}
