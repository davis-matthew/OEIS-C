/*
 * A287276  Number of set partitions of [n] such that for each block all
 *          absolute differences between consecutive elements are <= four.
 * https://oeis.org/A287276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of set partitions of [n] where * in each block the
 *            difference between consecutive elements is <= 4. * This is
 *            computed by a DP over the active blocks’ last processed *
 *            element distances, using a 5-bit mask (distances 0..4)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 5-bit mask: bit d is set if there is a block whose last element
     * is (current_position - 1) - d.  Distance 0 means the block ends
     * at the previous element.  Bits 0..4 correspond to distances 0..4. */
    const int MASK = 0x1F;   /* 5 bits */
    seq_t cur[32] = {0}, nxt[32] = {0};

    cur[0] = 1;             /* empty partition before any element */

    for (idx_t pos = 1; pos <= n; ++pos) {
        /* reset next state */
        for (int m = 0; m < 32; ++m) nxt[m] = 0;

        for (int m = 0; m < 32; ++m) {
            seq_t ways = cur[m];
            if (!ways) continue;

            /* Option 1: start a new block with element 'pos' */
            int newmask = ((m << 1) & MASK) | 1;
            nxt[newmask] = nxt[newmask] + ways;

            /* Options 2: extend one of the existing blocks
             * whose last element is within 4 of (pos-1). */
            for (int d = 0; d <= 3; ++d) {
                if (m & (1 << d)) {          /* block with distance d exists */
                    int shifted = ((m << 1) & MASK);
                    int nm = shifted & ~(1 << (d + 1)); /* remove shifted bit */
                    nm |= 1;                   /* new block ends at current pos */
                    nxt[nm] = nxt[nm] + ways;
                }
            }
        }

        /* swap current and next */
        for (int m = 0; m < 32; ++m) cur[m] = nxt[m];
    }

    /* Sum over all final states */
    seq_t result = 0;
    for (int m = 0; m < 32; ++m) result = result + cur[m];
    return result;
}
