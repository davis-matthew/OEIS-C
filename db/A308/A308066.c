/*
 * A308066  Number of triangles with perimeter n whose side lengths are even.
 * https://oeis.org/A308066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is odd; for even n let p = n/2. Then a(n) = (p*p +
 *            24)/48 if p is even a(n) = ((p+3)*(p+3) + 24)/48 if p is odd
 *            (This is equivalent to a(n) = A005044(p) where p = n/2.)
 * Range:     u8 n <= 217, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return 0 for odd perimeters */
    if (n & 1)
        return (seq_t)0;

    /* p = perimeter/2, the triangle perimeter with side lengths halved */
    seq_t p = (seq_t)n / 2;

    if ((p & 1) == 0) {          /* p is even  */
        return (p * p + (seq_t)24) / (seq_t)48;
    } else {                     /* p is odd   */
        seq_t t = p + 3;
        return (t * t + (seq_t)24) / (seq_t)48;
    }
}
