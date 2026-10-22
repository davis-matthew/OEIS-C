/*
 * A304384  a(n) = 168*2^n - 26 (n>=1).
 * https://oeis.org/A304384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 168*2^n - 26
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;
    seq_t val = pow2(n);          /* 2^n */
    val *= (seq_t)168;            /* 168*2^n */
    val -= (seq_t)26;             /* -26 */
    return val;
}
