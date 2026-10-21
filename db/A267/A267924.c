/*
 * A267924  Decimal representation of the n-th iteration of the "Rule 245"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n+1) - 5 (for n > 0) a(0) = 1 This is equivalent to
 *            the recurrence a(n) = 5*a(n-1) - 4*a(n-2) with a(0)=1, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    /* Compute 2^(2*n+1) using the provided pow2 helper.
       idx_t is unsigned, so 2*n+1 fits for the relevant range of n. */
    idx_t exp = (idx_t)(2 * (idx_t)n + 1);
    seq_t p = pow2(exp);           /* 2^(2*n+1) */
    return p - (seq_t)5;           /* subtract 5 to get a(n) */
}
