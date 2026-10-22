/*
 * A343229  A binary encoding of the digits "-1" in balanced ternary
 *          representation of n.
 * https://oeis.org/A343229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 6561, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced ternary to binary encoding.
   For an index n ≥ 0, a(n) is the integer whose binary
   representation has a 1 in position k iff the balanced ternary
   representation of n has a digit -1 in the position k
   (k = 0 for the least‑significant digit).  All other positions
   are 0.  This implements OEIS A343229. */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n;          /* working copy of the index          */
    idx_t i = 0;          /* current digit position             */
    seq_t  res = 0;       /* result to return                   */

    while (m != 0) {
        /* obtain next balanced ternary digit */
        int r = (int)(m % 3);
        m = m / 3;
        if (r == 2) {          /* 2 maps to -1 with a carry */
            r = -1;
            m += 1;
        }

        /* set bit if digit is -1 */
        if (r == -1) {
            res |= ((seq_t)1 << i);
        }

        ++i;
    }
    return res;
}
