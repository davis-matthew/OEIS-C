/*
 * A288762  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 501", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1,a(1)=2,a(2)=1,a(3)=14,a(4)=3,a(5)=62; for n>5, a(n)=3
 *            when n is even, otherwise a(n)=2*(2^n-2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)14;
    if (n == 4) return (seq_t)3;
    if (n == 5) return (seq_t)62;

    if ((n & 1) == 0)
        return (seq_t)3;

    seq_t t = pow2(n);   /* 2^n */
    t -= (seq_t)2;       /* 2^n - 2 */
    return t << 1;       /* 2 * (2^n - 2) */
}
