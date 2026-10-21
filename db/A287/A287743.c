/*
 * A287743  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 342", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^{⌊(n+1)/2⌋} * (10^{2*(⌊n/4⌋+1)} – 1)/99, n ≥ 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* integer divisions and shifts */
    idx_t b  = n >> 2;               /* floor(n/4)          */
    idx_t tz = (n + 1) >> 1;         /* floor((n+1)/2)      */
    idx_t exp1 = 2 * (b + 1);        /* 2*(b+1)             */

    /* compute (10^{exp1} - 1) / 99 */
    seq_t prefix = (pow10(exp1) - (seq_t)1) / (seq_t)99;

    /* multiply by 10^{tz} to append trailing zeros in the decimal representation */
    return prefix * pow10(tz);
}
