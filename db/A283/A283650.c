/*
 * A283650  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 705", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=7. For n>2: - if n is even: a(n) = 2*(2^n -
 *            2) = 2^(n+1) - 4 - if n is odd : a(n) = 2^(n+1) - 3 The function
 *            uses seq_pow from utils/seq_pow.h to compute 2^(n+1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)7;

    /* Compute 2^(n+1) using seq_pow */
    seq_t two_pow = seq_pow((seq_t)2, n + (idx_t)1);

    if ((n & (idx_t)1) == 0)          /* even n */
        return two_pow - (seq_t)4;
    else                              /* odd n */
        return two_pow - (seq_t)3;
}
