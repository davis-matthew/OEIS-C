/*
 * A273153  a(n) = Numerator of (0 followed by 1's) - n/2^n.
 * https://oeis.org/A273153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (2^n - n) / 2^n after reduction * = (2^n -
 *            n) >> v2(n), with a(0) := 0
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;

    /* Find v2(n), the exponent of 2 dividing n */
    idx_t k = 0, t = n;
    while ((t & 1u) == 0)
    {
        t >>= 1;
        ++k;
    }

    seq_t val = pow2(n);      /* 2^n */
    val -= (seq_t)n;          /* 2^n - n */
    val >>= k;                /* divide by 2^v2(n) */

    return val;
}
