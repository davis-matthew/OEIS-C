/*
 * A380996  a(n) is the number of vertices in the n-fold iterated barycentric
 *          subdivision of a triangle (or 2-simplex).
 * https://oeis.org/A380996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6^n + 3*2^n)/2 + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* 6^n */
    seq_t pow6 = pow_int((seq_t)6, n);
    /* 2^n */
    seq_t pow2 = pow_int((seq_t)2, n);

    /* (6^n + 3*2^n)/2 + 1 */
    seq_t res = pow6 + 3 * pow2;
    res /= 2;
    res += 1;
    return res;
}
