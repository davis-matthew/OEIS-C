/*
 * A280610  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 294", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A280610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 - 3*(-1)^n + 2^n for n>3; a(0)=1, a(1)=3, a(2)=1,
 *            a(3)=7
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* initial values */
    static const seq_t init[4] = { (seq_t)1,
                                   (seq_t)3,
                                   (seq_t)1,
                                   (seq_t)7 };

    if (n <= 3)
        return init[n];

    /* for n >= 4 : a(n) = 4 - 3*(-1)^n + 2^n
       = 1 + 2^n (if n even), 7 + 2^n (if n odd) */
    seq_t res = pow2(n);
    if ((n & 1) == 0)
        return res + (seq_t)1;
    else
        return res + (seq_t)7;
}
