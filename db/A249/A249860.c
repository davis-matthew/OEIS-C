/*
 * A249860  a(n) = Least common multiple of n + 3 and n - 3.
 * https://oeis.org/A249860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n+3, n-3) with the lcm defined for absolute values.
 *            For n = 1, 2 we treat n-3 as -2, -1 respectively; for n = 3 we
 *            return 0
 * Range:     u8 n <= 19, u16 n <= 259, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so n >= 1 for valid queries. */
    seq_t p = (seq_t)n + (seq_t)3;           /* p = n+3, always positive */

    seq_t q;
    if (n >= 3)
        q = (seq_t)n - (seq_t)3;            /* n-3, non‑negative for n>=3 */
    else
        q = (seq_t)3 - (seq_t)n;            /* |n-3| for n=1,2 */

    if (q == (seq_t)0)                      /* lcm(x,0) = 0 by convention */
        return (seq_t)0;

    seq_t g = gcd(p, q);                    /* greatest common divisor */
    seq_t l = p / g * q;                    /* lcm = p/gcd * q */
    return l;
}
