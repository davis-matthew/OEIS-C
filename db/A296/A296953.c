/*
 * A296953  Number of bisymmetric, quasitrivial, and order-preserving binary
 *          operations on the n-element set {1,...,n}.
 * https://oeis.org/A296953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(n)=3*2^(n-1)-2 for n>0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t val = pow2(n - 1);   /* 2^(n-1) */
    val *= (seq_t)3;           /* 3*2^(n-1) */
    val -= (seq_t)2;           /* -2 */
    return val;
}
