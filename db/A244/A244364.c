/*
 * A244364  a(n) is the least integer m > 1 such that n is the largest number
 *          of identical digits that can end m^k for positive integer k.
 * https://oeis.org/A244364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hard, more
 * Formula:   a(n) = 10*2^(n-1)-1 for n>5; a(1)=3, a(2)=4, a(3)=2, a(4)=33,
 *            a(5)=17
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* initial values for the first 5 indices */
    static const seq_t init[] = { 3, 4, 2, 33, 17 };
    if (n >= 1 && n <= 5)
        return init[n - 1];

    /* closed‑form for n > 5 */
    seq_t p = pow2(n - 1);          /* 2^(n‑1) */
    p = mul_small(p, 10);           /* *10 */
    return p - 1;                   /* -1 */
}
