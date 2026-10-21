/*
 * A262710  Powers of -4.
 * https://oeis.org/A262710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-4)^n
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 1;   /* (-4)^0 = 1 */
    seq_t base   = 4;   /* 4 is used, sign dealt with at end */
    idx_t exp    = n;   /* exponent is unsigned */

    while (exp > 0) {
        if (exp & 1)          /* if the lowest bit is set, multiply result */
            result = result * base;
        base = base * base;   /* square base for next bit */
        exp >>= 1;            /* shift exponent right */
    }

    if (n & 1)                /* apply (-1)^n */
        result = -result;

    return result;
}
