/*
 * A247618  Start with a single square; at n-th generation add a square at
 *          each expandable vertex; a(n) is the sum of all label values at
 *          n-th generation. (See comment for construction rules.)
 * https://oeis.org/A247618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+3) - 4*n - 7 (OEIS A247618)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Computes A247618 at index n. */
seq_t a(idx_t n)
{
    /* 2^(n+3) */
    seq_t pow_val = pow2(n + 3);

    /* a(n) = 2^(n+3) - 4*n - 7 */
    seq_t val = pow_val;
    val -= (seq_t)4 * n;
    val -= 7;
    return val;
}
