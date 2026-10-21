/*
 * A296757  Numbers whose base-15 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          > #(falls); see Comments.
 * https://oeis.org/A296757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rises_gt_falls
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A296757: numbers whose base‑15 digits have more rises than falls.
   The function scans positive integers, counts rises and falls in
   base‑15 representation, and returns the n‑th number that satisfies
   rises > falls (offset 1). */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/rises_gt_falls.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset starts at 1 */

    seq_t val = (seq_t)1;
    idx_t  count = (idx_t)0;

    while (count < n) {
        if (rises_gt_falls(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
    /* unreachable for valid input */
    return val;
}
