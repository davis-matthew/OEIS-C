/*
 * A287131  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 245", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 for n>5 and even; a(n) = 2^(n+1) - 4 for n>5 and odd;
 *            a(0)=1, a(1)=2, a(2)=0, a(3)=14, a(4)=3, a(5)=62
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static const seq_t init[6] = {1, 2, 0, 14, 3, 62};

seq_t a(idx_t n)
{
    if (n <= 5)          return init[n];
    if ((n & 1) == 0)    return (seq_t)3;          /* even   */
    /* odd    */
    seq_t val = pow2(n + 1);   /* 2^(n+1)   */
    val -= (seq_t)4;           /* subtract 4 */
    return val;
}
