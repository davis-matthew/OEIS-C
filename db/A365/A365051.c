/*
 * A365051  a(n) = |Aut^n(C_40)|: order of the group obtained by applying G ->
 *          Aut(G) n times to the cyclic group of order 40.
 * https://oeis.org/A365051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(0)=40, a(1)=16, a(2)=192, a(n)=9*2^(2n+1) for n>=3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)40;
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)192;

    /* For n>=3: a(n) = 9 * 2^(2n+1) */
    idx_t exp = 2 * n + 1;
    seq_t two_pow = pow2(exp);
    return two_pow * (seq_t)9;
}
