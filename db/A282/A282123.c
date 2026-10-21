/*
 * A282123  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 430", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   if n is odd: a(n) = 2^{n+1} - 1 * if n is even: a(n) = (5·2^n -
 *            2)/3
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 63, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* ensure n is a non‑negative index – the function is only called with n ≥ 0
     * It uses the closed form derived from the recurrence:
     *      a(n) = (-5 - (-2)^n + (-1)^n + 11·2^n) / 6
     * which simplifies to the two branches shown above.  */
    if (n & 1)                     /* odd index */
    {
        seq_t p = pow2(n + 1);     /* 2^{n+1}  */
        return p - (seq_t)1;
    }
    else                            /* even index */
    {
        seq_t p   = pow2(n);       /* 2^n     */
        seq_t v   = (seq_t)5 * p - (seq_t)2;
        return v / (seq_t)3;
    }
}
