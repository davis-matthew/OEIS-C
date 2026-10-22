/*
 * A356860  a(n) is the number of digits in the product of the first n numbers
 *          not divisible by 5.
 * https://oeis.org/A356860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 140, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A356860
   a(n) = number of digits in the product of the first n positive integers not divisible by 5. */
#include <math.h>   /* log10l, floorl */

seq_t a(idx_t n)
{
    /* Empty product has one digit, namely 1. */
    if (n == 0) return (seq_t)1;

    long double dlog = 0.0L;      /* running sum of base‑10 logarithms   */
    idx_t count = 0;              /* how many qualifying numbers processed */
    idx_t x     = 1;              /* current candidate integer            */

    while (count < n) {
        if (x % 5u != 0u) {        /* skip multiples of 5                */
            dlog += log10l((long double)x);
            ++count;
        }
        ++x;
    }

    /* number of decimal digits = floor(log10(product)) + 1     */
    idx_t digits = (idx_t)(floorl(dlog) + 1.0L);
    return (seq_t)digits;
}
