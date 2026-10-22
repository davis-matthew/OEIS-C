/*
 * A348227  Coordination sequence for Wilkinson's 123-circle packing with
 *          respect to a circle of radius 1.
 * https://oeis.org/A348227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 9*n + (n - 2)*(-1)^n - 4 for n > 3 with a(0)=1, a(1)=4,
 *            a(2)=12, a(3)=20 This is equivalent to: if n even: a(n) = 10*n -
 *            6 if n odd : a(n) = 8*n - 2
 * Range:     u8 n <= 27, u16 n <= 6555, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)20;

    seq_t nn = (seq_t)n;
    if ((n & 1) == 0)
        return (seq_t)10 * nn - 6;
    else
        return (seq_t)8 * nn - 2;
}
