/*
 * A312561  Coordination sequence Gal.6.247.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A312561(0) = 1. For n > 0: if n is odd: k = (n-1)/2 a(n) = 10*k
 *            + 4 - 2*floor(k/5) if n is even: k = n/2 a(n) = 10*k -
 *            2*floor(k/5) - 2*[(k mod 5 == 1) or (k mod 5 == 2)] This
 *            reproduces:
 *            1,4,8,14,18,24,30,34,40,44,48,52,56,62,66,72,78,82,88,92,96,100,104,110,
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    if (n & 1)          /* odd n */
    {
        idx_t k = n >> 1;            /* (n-1)/2 */
        seq_t v = (seq_t)k * 10u + 4u;
        seq_t sub = (seq_t)2u * (seq_t)(k / 5);
        return v - sub;
    }
    else                /* even n, n > 0 */
    {
        idx_t k = n >> 1;            /* n/2 */
        seq_t v = (seq_t)k * 10u;
        seq_t sub = (seq_t)2u * (seq_t)(k / 5);
        idx_t r = k % 5;
        if (r == 1 || r == 2)
            sub += 2u;
        return v - sub;
    }
}
