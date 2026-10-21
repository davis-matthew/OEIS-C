/*
 * A282442  a(n) is the smallest step size that does not occur on a staircase
 *          of n steps when following the following procedure: Take steps of
 *          length 1 up a staircase until you can't step any further, then
 *          take steps of length 2 down until you can't step any further, and
 *          so on.
 * https://oeis.org/A282442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 268, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: simulate the staircase walk for step sizes 1..n+1
 * and find the smallest step size that never occurs. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Allocate a boolean array to record which step sizes were used.
       Index 0 is unused; we use indices 1..n+1. */
    bool *used = (bool *)calloc((size_t)(n + 2), sizeof(bool));
    if (!used) return (seq_t)0; /* Out of memory guard */

    idx_t pos = 0; /* start at the bottom of the staircase */
    for (idx_t k = 1; k <= n + 1; ++k) {
        bool stepped = false;
        if (k & 1U) { /* odd k: move up */
            while (pos + k <= n) {
                pos += k;
                stepped = true;
            }
        } else { /* even k: move down */
            while (pos >= k) { /* pos - k >= 0 */
                pos -= k;
                stepped = true;
            }
        }
        if (stepped) used[k] = true;
    }

    /* Find the smallest step size that did not occur. */
    idx_t ans = 1;
    while (ans <= n + 1 && used[ans]) ++ans;

    free(used);
    return (seq_t)ans;
}
