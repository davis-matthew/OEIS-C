/*
 * A364904  a(n) = |Aut^n(C_32)|: order of the group obtained by applying G ->
 *          Aut(G) n times to the cyclic group of order 32.
 * https://oeis.org/A364904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(0) = 32 a(1) = a(2) = 16 a(3) = 64 a(n) = 3 * 2^(2*n-1) for n
 *            >= 4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)32;
    if (n == (idx_t)1 || n == (idx_t)2) return (seq_t)16;
    if (n == (idx_t)3) return (seq_t)64;

    /* n >= 4 */
    idx_t exp = 2 * n - (idx_t)1;   /* exponent for 2 */
    seq_t pow_val = pow2(exp);      /* 2^exp */
    return (seq_t)3 * pow_val;
}
