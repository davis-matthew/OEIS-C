/*
 * A282286  Least common multiple of 7*n+1 and 7*n-1.
 * https://oeis.org/A282286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(7*n-1, 7*n+1)
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                  /* a(0) is defined as 1 */

    /* compute 7*n-1 and 7*n+1 as positive values (n>0) */
    seq_t x = (seq_t)7 * n - 1;
    seq_t y = (seq_t)7 * n + 1;

    /* lcm(a,b) = a/gcd(a,b) * b */
    seq_t g = gcd(x, y);
    return (x / g) * y;
}
