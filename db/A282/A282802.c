/*
 * A282802  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 507", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=1; for n>1 even: a(n)=2^(n+1)-1; for n>1 odd:
 *            a(n)=2*(2^n-2)/3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;

    if ((n & 1) == 0) {           /* even */
        seq_t val = pow2(n + 1);   /* 2^(n+1) */
        return val - 1;
    } else {                       /* odd */
        seq_t val = pow2(n);       /* 2^n */
        seq_t r = val - 2;
        r = r * 2;
        return r / 3;
    }
}
