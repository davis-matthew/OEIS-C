/*
 * A330036  The length of the largest run of 0's in the binary expansion of n
 *          + the length of the largest run of 1's in the binary expansion of
 *          n.
 * https://oeis.org/A330036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A087117(n) + A038374(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   max_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A087117(n): length of the longest run of 0's in the binary representation of n
 *  A038374(n): length of the longest run of 1's in the binary representation of n
 */
#include <stdbool.h>
#include "../utils/max_idx_t.h"

/* Return a(n).  n is the OEIS index (offset 0).  */
seq_t
a(idx_t n)
{
    /* Special case: n == 0 → binary "0"  → longest run of 0's = 1,
       longest run of 1's = 0, sum = 1. */
    if (n == 0)
        return (seq_t)1;

    /* Find the position of the most‑significant 1 bit. */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        msb++;
    }

    idx_t max0 = 0;          /* longest run of 0's so far */
    idx_t max1 = 0;          /* longest run of 1's so far */
    idx_t curr = 0;          /* length of the current run */
    bool in_one = false;     /* true if the current run consists of 1's */

    /* Scan bits from most significant to least significant. */
    for (idx_t i = msb;;) {
        bool bit = (n >> i) & 1U;

        if (bit) {                    /* we are on a '1' bit */
            if (!in_one) {            /* start of a run of 1's */
                if (in_one)          /* this never happens here */
                    max1 = max_idx_t(max1, curr);
                else
                    max0 = max_idx_t(max0, curr);
                curr = 1;
                in_one = true;
            } else {                  /* continuation of '1's */
                curr++;
            }
        } else {                      /* we are on a '0' bit */
            if (in_one) {            /* run of 1's ended */
                max1 = max_idx_t(max1, curr);
                curr = 1;
                in_one = false;
            } else {                 /* continuation of '0's */
                if (curr == 0)            /* first '0' after a run of 1's */
                    curr = 1;
                else
                    curr++;
            }
        }

        if (i == 0)
            break;                      /* finished processing all bits */

        --i;
    }

    /* Record the final run. */
    if (in_one)
        max1 = max_idx_t(max1, curr);
    else
        max0 = max_idx_t(max0, curr);

    return (seq_t)max0 + (seq_t)max1;
}
