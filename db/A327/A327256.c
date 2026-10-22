/*
 * A327256  a(n) = floor(2*n*r) - 2*floor(n*r), where r = sqrt(8).
 * https://oeis.org/A327256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*n*sqrt(8)) - 2*floor(n*sqrt(8))
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 4300, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert n to the arithmetic type of the sequence value. */
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;                     /* n² */

    /* floor(4*n*sqrt(2)) = floor(sqrt(32*n²)) */
    seq_t t1   = (seq_t)32 * n2;
    seq_t int1 = floor_sqrt(t1);

    /* floor(2*n*sqrt(2)) = floor(sqrt(8*n²)) */
    seq_t t2   = (seq_t)8 * n2;
    seq_t int2 = floor_sqrt(t2);

    return int1 - (int2 + int2);
}
