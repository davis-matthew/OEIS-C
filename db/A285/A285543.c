/*
 * A285543  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 35", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - (n even ? 2^(n/2) : 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n + 1);            /* 2^(n+1) */
    if ((n & 1) == 0) {                 /* n is even */
        val -= pow2(n / 2);             /* subtract 2^(n/2) */
    } else {                            /* n is odd */
        val -= (seq_t)1;                /* subtract 1 */
    }
    return val;
}
