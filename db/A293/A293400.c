/*
 * A293400  Greatest integer k such that k/n^2 < (1 + sqrt(5))/2 (the golden
 *          ratio).
 * https://oeis.org/A293400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((1+sqrt(5))/2) * n^2)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 94, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute n^2.  Casting to seq_t before multiplication guarantees
       that the multiplication is performed in the correct type.      */
    seq_t nn = (seq_t)n * (seq_t)n;      // nn = n^2

    /* Compute s = floor(sqrt(5) * n^2) = floor(sqrt(5 * n^4)).
       Using floor_sqrt on 5*nn*nn gives the exact integer part.     */
    seq_t t = (seq_t)5 * nn * nn;        // t = 5 * n^4
    seq_t s = floor_sqrt(t);             // s = floor(sqrt(t))

    /* Final result: floor((nn + s) / 2) = floor(((1+sqrt(5))/2) * n^2). */
    return (nn + s) / 2;
}
