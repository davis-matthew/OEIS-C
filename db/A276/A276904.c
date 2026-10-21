/*
 * A276904  Number of positive walks with n steps {-3,-2,-1,0,1,2,3} starting
 *          at the origin, ending at altitude 2, and staying strictly above
 *          the x-axis.
 * https://oeis.org/A276904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A276904 – Number of positive walks with steps {-3,-2,-1,0,1,2,3}
   starting at the origin, ending at altitude 2, and staying strictly
   above the x-axis.
   The value is computed via a dynamic program over the altitude
   after each step.  Since the steps set is symmetric about 0,
   the transition from altitude h to the next step is the sum of the
   previous states in the interval [h-3, h+3].  This yields a
   linear‑time sliding window update that runs in O(n²) operations,
   easily fast enough for n up to 10 000 even with big‑integer
   types.
*/

#include <stdlib.h>

/* Compute the n-th term of A276904.  */
seq_t a(idx_t n)
{
    /* The sequence is trivial for n = 0. */
    if (n == 0) return 0;

    /* Size of the altitude array needed for the longest walk. */
    const size_t max_size = 3 * (size_t)n + 1;

    /* Two buffers for the dynamic programming rows. */
    seq_t *dp0 = (seq_t*)calloc(max_size, sizeof(seq_t));
    seq_t *dp1 = (seq_t*)calloc(max_size, sizeof(seq_t));
    if (!dp0 || !dp1) {          /* allocation failure – return 0 */
        free(dp0); free(dp1);
        return 0;
    }

    /* Initial state: at the origin with 0 steps taken. */
    dp0[0] = (seq_t)1;
    size_t prev_max = 0;          /* maximal altitude reached in dp0 */

    for (idx_t step = 1; step <= n; ++step) {
        size_t new_max = 3 * (size_t)step;       /* new maximal altitude */

        /* Sliding‑window sum over dp0[h-3 .. h+3] */
        seq_t window_sum = 0;
        long long left  = 0;          /* leftmost index in current window */
        long long right = -1;         /* rightmost index (initially none) */

        for (size_t h = 1; h <= new_max; ++h) {
            long long new_left  = (h >= 3) ? (long long)(h - 3) : 0;
            long long new_right = (h + 3 <= prev_max)
                                  ? (long long)(h + 3)
                                  : (long long)prev_max;

            /* Expand the window to the right. */
            while (right < new_right) {
                ++right;
                window_sum += dp0[(size_t)right];
            }
            /* Shrink the window from the left. */
            while (left  < new_left) {
                window_sum -= dp0[(size_t)left];
                ++left;
            }
            dp1[(size_t)h] = window_sum;
        }
        dp1[0] = 0;                  /* staying at altitude 0 is forbidden */

        /* Prepare for the next iteration. */
        seq_t *tmp   = dp0; dp0 = dp1; dp1 = tmp;
        prev_max = new_max;
    }

    seq_t result = 0;
    if (prev_max >= 2) result = dp0[2];

    /* Clean up */
    free(dp0);
    free(dp1);
    return result;
}
