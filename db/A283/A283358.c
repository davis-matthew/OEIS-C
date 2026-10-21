/*
 * A283358  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 621", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n > 4, a(n) = 2^n – 15 if n even, 2^n – 3 if n odd. Initial
 *            values: a(0)=a(1)=a(2)=1, a(3)=13, a(4)=17
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly */
    if (n <= 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)13;
    if (n == 4)
        return (seq_t)17;

    /* For n ≥ 5 use the closed‑form expression 2^n – c. */
    seq_t power = pow2(n);          /* 2^n */
    if ((n & 1) == 0)              /* even n */
        return power - (seq_t)15;
    else                           /* odd n */
        return power - (seq_t)3;
}
