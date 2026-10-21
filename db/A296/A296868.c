/*
 * A296868  Numbers whose base-5 digits d(m), d(m-1), ..., d(0) have #(pits) >
 *          #(peaks); see Comments.
 * https://oeis.org/A296868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth natural number whose base‑5 digits contain
 *            strictly more pits than peaks (found by exhaustive search)
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_pits_gt_peaks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_pits_gt_peaks.h"

/* Return the nth element (1‑based) of the sequence A296868.  */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but avoid underflow when n=1 */
        return (seq_t)0;

    seq_t val = (seq_t)1;    /* start from 1 */
    idx_t count = 0;         /* how many qualifying numbers found */
    while (1) {
        if (is_pits_gt_peaks(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}
