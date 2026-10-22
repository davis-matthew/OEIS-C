/*
 * A383450  2nd diagonal (from right) in A104978.
 * https://oeis.org/A383450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n + 4)! / (2 * n! * (3 + 2*n)!)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent expression used: a(n) = binom(3*n + 4, 2*n + 3) * (n + 1) / 2 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(3n+4, 2n+3) */
    idx_t top   = 3 * n + 4;
    idx_t below = 2 * n + 3;
    seq_t val   = binom(top, below);          /* val = (3n+4)! / ((2n+3)! * (n+1)!) */

    /* Multiply by (n+1) and then divide by 2 to preserve integer division */
    val = val * (seq_t)(n + 1);
    val = val / (seq_t)2;

    return val;
}
