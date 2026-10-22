/*
 * A357996  a(n) is the number of times in the format hh:mm that can be
 *          represented in a 7-segment display by using only n segments
 *          (version A006942).
 * https://oeis.org/A357996
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of times hh:mm can be displayed using exactly n segments
   on a standard 7‑segment display for each n ≥ 8.  The sequence is zero
   outside the range 8 … 26. */
#include <stdbool.h>

static const unsigned char seg[10] = {6,2,5,5,4,5,6,3,7,6};

seq_t a(idx_t n)
{
    /* static table for segment sums up to 26 (max for all hh:mm) */
    static seq_t counts[27] = {0};
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        for (idx_t h = 0; h < 24; ++h) {
            unsigned int ht = h / 10;
            unsigned int hu = h % 10;
            for (idx_t m = 0; m < 60; ++m) {
                unsigned int mt = m / 10;
                unsigned int mu = m % 10;
                unsigned int total = seg[ht] + seg[hu] + seg[mt] + seg[mu];
                /* total is guaranteed to be <= 26 */
                counts[total] = counts[total] + 1;
            }
        }
    }

    if (n <= 26) {
        return counts[n];
    }
    /* Outside the valid segment range the count is zero */
    return 0;
}
