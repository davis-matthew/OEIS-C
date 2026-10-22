/*
 * A304516  a(n) = 192*2^n - 273 (n>=1).
 * https://oeis.org/A304516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 192*2^n - 273 (n>=1)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t res = 192;
    res *= pow2(n);   /* compute 192 * 2^n */
    res -= 273;       /* subtract 273 */
    return res;       /* result is a(n) for n >= 1 */
}
