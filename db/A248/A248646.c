/*
 * A248646  Expansion of g.f. x*(x^3+x^2+x+2)/(1-2*x).
 * https://oeis.org/A248646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2, a(2)=5, a(3)=11, a(4)=23, a(n)=23*2^(n-4) for n>4
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return 2;
    if (n == 2) return 5;
    if (n == 3) return 11;
    if (n == 4) return 23;
    /* n > 4 */
    return pow2(n - 4) * ((seq_t)23);
}
