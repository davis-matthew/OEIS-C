/*
 * A356880  Squares that can be expressed as the sum of two powers of two (2^x
 *          + 2^y).
 * https://oeis.org/A356880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) if n is odd; a(n) = 9 * 2^(n-2) if n is even
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n & 1)  /* odd n */
    {
        return pow2(n + 1);
    }
    else        /* even n (n >= 2) */
    {
        seq_t t = pow2(n - 2);
        return t * (seq_t)9;
    }
}
