/*
 * A334083  Integers m such that all binomial coefficients C(m,k), with
 *          0<=k<=m, are practical numbers.
 * https://oeis.org/A334083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A334083: Integers m such that all binomial coefficients C(m,k) are practical.
   The sequence consists of the powers of two except 2^3 (=8) and 2^9 (=512).
   Hence a(n) = 2^e(n) where 
        e(n) = n-1            for n <= 3,
        e(n) = n              for 4 <= n <= 8,
        e(n) = n + 1          for n >= 9.
*/
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t e;
    if (n <= 3)
        e = n - 1;
    else if (n <= 8)
        e = n;
    else
        e = n + 1;
    return pow2(e);
}
