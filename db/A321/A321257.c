/*
 * A321257  Start with an equilateral triangle, and repeatedly append along
 *          the triangles of the previous step equilateral triangles with half
 *          their side length that do not overlap with any prior triangle;
 *          a(n) gives the number of triangles appended at n-th step.
 * https://oeis.org/A321257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(2^(n-1) + 3*(2^(n-1)-n)) for n>1; a(1)=1
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* Calculate 2^(n-1) as a seq_t. */
    seq_t pow = pow2(n - 1);

    /* Compute 2^(n-1) - n. */
    seq_t sub = pow - (seq_t)n;

    /* 3 * (2^(n-1) - n). */
    seq_t mul3sub = 3 * sub;

    /* 2^(n-1) + 3*(2^(n-1)-n). */
    seq_t sum = pow + mul3sub;

    /* Finally 3 * ( 2^(n-1) + 3*(2^(n-1)-n) ). */
    return 3 * sum;
}
