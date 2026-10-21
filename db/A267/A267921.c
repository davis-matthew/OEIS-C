/*
 * A267921  Decimal representation of the n-th iteration of the "Rule 243"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n+1) - 3 (for n > 0, a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Recurrence: a(0)=1 , a(1)=5 , a(n)=5*a(n-1)-4*a(n-2) for n>2 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* exponent = 2*n + 1, computed in the unsigned integer type */
    idx_t e = (idx_t)((idx_t)2 * n + (idx_t)1);

    /* 2^(2*n+1) */
    seq_t val = pow2(e);

    /* subtract 3 */
    return val - (seq_t)3;
}
