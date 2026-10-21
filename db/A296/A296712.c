/*
 * A296712  Numbers whose base-10 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          = #(falls); see Comments.
 * https://oeis.org/A296712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   equal_rises_falls
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th element of OEIS A296712:
 * Numbers whose decimal digits have an equal number of rises and falls.
 * a(1) = 1.  The function searches natural numbers in increasing order
 * and counts rises (digit(i) < digit(i+1)) and falls (digit(i) > digit(i+1)).
 */
#include <stdbool.h>
#include "../utils/equal_rises_falls.h"

seq_t a(idx_t n)
{
    /* Find the n-th integer satisfying the rise/fall condition. */
    idx_t found = 0;
    seq_t cur = (seq_t)0;
    for (;;) {                      /* loop until we have n values */
        cur++;
        if (equal_rises_falls(cur)) {
            found++;
            if (found == n)
                return cur;
        }
    }
}
