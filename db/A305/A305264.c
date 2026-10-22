/*
 * A305264  a(n) = 836*2^n - 771.
 * https://oeis.org/A305264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 836*2^n - 771
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 22, u64 n <= 54, u128 n <= 118,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t t = pow2(n);          /* 2^n */
    t *= (seq_t)836;            /* 836*2^n */
    t -= (seq_t)771;            /* minus 771 */
    return t;
}
