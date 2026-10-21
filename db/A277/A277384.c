/*
 * A277384  Least common multiple of n + 4 and n - 4.
 * https://oeis.org/A277384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n+4, n-4)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (uses the provided gcd implementation) */
#include "../utils/gcd.h"

/* Compute least common multiple of n+4 and n-4.
   For n ≤ 4, n-4 may be negative or zero, so we use the absolute value.
   If |n-4| is zero (i.e., n = 4), the lcm is defined as 0. */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n + (seq_t)4;          /* n+4  (always positive)   */
    seq_t y;                               /* |n-4|  (absolute value) */

    if (n >= 4)
        y = (seq_t)(n - 4);                /* positive difference    */
    else
        y = (seq_t)(4 - n);                /* negative difference    */

    if (y == 0)                            /* n == 4 */
        return (seq_t)0;

    seq_t g = gcd(x, y);                   /* gcd of the two numbers */
    return (x / g) * y;                    /* lcm = x/g * y           */
}
