/*
 * A342074  Number of n-colorings of the vertices of the 6-dimensional cross
 *          polytope such that no two adjacent vertices have the same color.
 * https://oeis.org/A342074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 5)*(n - 4)*(n - 3)*(n - 2)*(n - 1)*n* (190435 -
 *            149879*n + 49144*n² - 8605*n³ + 850*n⁴ - 45*n⁵ + n⁶)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 45, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 5)
        return 0;

    seq_t x  = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t x4 = x3 * x;
    seq_t x5 = x4 * x;
    seq_t x6 = x5 * x;

    seq_t poly = 190435
                 - 149879 * x
                 + 49144 * x2
                 - 8605 * x3
                 + 850 * x4
                 - 45 * x5
                 + x6;

    seq_t prod = (x - 5) * (x - 4) * (x - 3) * (x - 2) * (x - 1) * x;
    return prod * poly;
}
