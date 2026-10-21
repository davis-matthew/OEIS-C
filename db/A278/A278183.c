/*
 * A278183  Number of n X 2 0..3 arrays with every element plus 1 mod 4 equal
 *          to some element at offset (-1,-1) (-1,0) (-1,1) (0,-1) (0,1) or
 *          (1,0), with upper left element zero.
 * https://oeis.org/A278183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence: a(n) = 8*a(n-1) - 4*a(n-2) + 15*a(n-3) -
 *            26*a(n-4) + 14*a(n-5) - 16*a(n-6) Initial values: a(1)=0,
 *            a(2)=3, a(3)=28, a(4)=200, a(5)=1532, a(6)=11794 This
 *            implementation computes a(n) for any n ≥ 1 using only integer
 *            arithmetic and an O(n) recurrence. No dynamic allocation is...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Initial five values for n = 1..6. */
    static const seq_t init[6] = {
        (seq_t)0,
        (seq_t)3,
        (seq_t)28,
        (seq_t)200,
        (seq_t)1532,
        (seq_t)11794
    };

    /* Return directly for the first six indices. */
    if (n <= 6) {
        return init[n - 1];
    }

    /* Keep the last six computed terms.  prev[0] = a(n‑6), ..., prev[5] = a(n‑1). */
    seq_t prev[6] = {
        init[0], init[1], init[2],
        init[3], init[4], init[5]
    };
    seq_t cur;

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute a(i) from the six preceding values. */
        cur = 8 * prev[5]
            - 4 * prev[4]
            + 15 * prev[3]
            - 26 * prev[2]
            + 14 * prev[1]
            - 16 * prev[0];

        /* Shift the window: drop the oldest value (prev[0]) and
           incorporate the new value (cur).  After the shift,
           prev[5] will hold a(i). */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = cur;
    }

    /* At this point, prev[5] holds a(n). */
    return prev[5];
}
