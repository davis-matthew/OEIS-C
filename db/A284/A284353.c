/*
 * A284353  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 899", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A284353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1)-1 if n even, 2^(n+1)-3 if n odd
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A284353.  */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n + 1));
    if ((n & 1) == 0)   /* n even */
        return pow2 - (seq_t)1;
    else                /* n odd */
        return pow2 - (seq_t)3;
}
