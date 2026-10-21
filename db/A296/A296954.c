/*
 * A296954  Expansion of x*(1 - x + 4*x^2) / ((1 - x)*(1 - 2*x)).
 * https://oeis.org/A296954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-1) - 4 for n>1; a(0)=0; a(1)=1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* For n >= 2: compute 3*2^(n-1) - 4 */
    seq_t val = pow2((idx_t)(n - 1));   /* 2^(n-1) */
    val *= (seq_t)3;                    /* 3*2^(n-1) */
    val -= (seq_t)4;                    /* minus 4 */
    return val;
}
