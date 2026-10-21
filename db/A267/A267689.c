/*
 * A267689  Decimal representation of the n-th iteration of the "Rule 199"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(1+2*n) - ((-1)^n + 2^(1+n))/3 - 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Handles the special case n = 0 separately.         */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t exp1 = 1 + (n << 1);   /* 1 + 2*n  */
    idx_t exp2 = 1 + n;          /* 1 + n    */

    seq_t t1 = pow2(exp1);       /* 2^(1+2*n) */
    seq_t t2 = pow2(exp2);       /* 2^(1+n)   */

    seq_t one = (seq_t)1;

    seq_t numerator;
    if ((n & 1) == 0) {          /* n even: (-1)^n = +1 */
        numerator = t2 + one;
    } else {                     /* n odd:  (-1)^n = -1 */
        numerator = t2 - one;
    }

    seq_t frac = numerator / (seq_t)3; /* integer division, guaranteed exact */

    return t1 - frac - one;
}
