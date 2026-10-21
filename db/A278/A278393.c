/*
 * A278393  Number of positive meanders (walks starting at the origin and
 *          ending at any altitude > 0 that never touch or go below the x-axis
 *          in between) with n steps from {-4,-3,-2,-1,0,1,2,3,4}.
 * https://oeis.org/A278393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = number of positive walks of length n using steps *
 *            {-4,-3,-2,-1,0,1,2,3,4} that never touch the x-axis after the
 *            first step. * Recurrence: let dp[i][h] be the number of such
 *            walks of length i ending at height h>0 * (dp[0][0] = 1,
 *            dp[0][h>0] = 0). Then * * dp[i+1][h] = Σ_{s=-4}^{4} dp[i][h-...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static memoisation state ------------------------------------------ */
static idx_t    max_computed = 0;   /* largest n already computed */
static seq_t  *ans_arr      = NULL;/* array of a(0)…a(max_computed) */
static size_t  ans_cap      = 0;   /* allocated size of ans_arr */

static seq_t *dp_prev = NULL;      /* dp values for previous length */
static idx_t  prev_max_h = 0;      /* largest height stored in dp_prev */

/* Ensure ans_arr is large enough to hold indices up to n. */
static void ensure_ans_cap(idx_t n)
{
        if ((size_t)n + 1 > ans_cap) {
                size_t new_cap = (size_t)n + 1;
                seq_t *tmp = (seq_t*)realloc(ans_arr, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                ans_arr = tmp;
                ans_cap = new_cap;
        }
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
        /* First call initialises the state */
        if (ans_cap == 0) {
                ans_cap = 1;
                ans_arr = (seq_t*)malloc(ans_cap * sizeof(seq_t));
                if (!ans_arr) exit(EXIT_FAILURE);
                ans_arr[0] = (seq_t)1;          /* a(0) = 1 */
                max_computed = 0;
                dp_prev = NULL;
                prev_max_h = 0;
        }

        /* Cache hit ------------------------------------------------------ */
        if (n <= max_computed)
                return ans_arr[n];

        /* Compute values from max_computed+1 up to n --------------------- */
        for (idx_t m = max_computed + 1; m <= n; ++m) {
                idx_t curr_max_h = 4 * m;               /* maximum possible height */
                /* Allocate and zero the array for current dp values */
                seq_t *dp_curr = (seq_t*)calloc(curr_max_h + 1, sizeof(seq_t));
                if (!dp_curr) exit(EXIT_FAILURE);

                seq_t total = 0;                         /* a(m) */

                if (m == 1) {                            /* first step: must be positive */
                        for (idx_t h = 1; h <= 4; ++h) {
                                dp_curr[h] = (seq_t)1;
                                total += dp_curr[h];
                        }
                } else {
                        /* Sliding window over dp_prev to compute
                         * dp_curr[h] = Σ_{s=-4..4} dp_prev[h-s]
                         * with h-s ≥ 1.  The window ranges from
                         * j = max(1, h-4) to j = min(prev_max_h, h+4). */
                        seq_t window_sum = 0;
                        idx_t win_start = 1, win_end = 0; /* window initially empty */

                        for (idx_t h = 1; h <= curr_max_h; ++h) {
                                idx_t new_end   = (h + 4 <= prev_max_h) ? (h + 4) : prev_max_h;
                                while (win_end < new_end) {
                                        ++win_end;
                                        window_sum += dp_prev[win_end];
                                }
                                idx_t new_start = (h >= 5) ? (h - 4) : 1;
                                while (win_start < new_start) {
                                        window_sum -= dp_prev[win_start];
                                        ++win_start;
                                }
                                dp_curr[h] = window_sum;
                                total += window_sum;
                        }
                }

                /* Store the computed a(m) */
                ensure_ans_cap(m);
                ans_arr[m] = total;

                /* Prepare for next iteration: the current dp becomes the previous dp */
                if (dp_prev) free(dp_prev);
                dp_prev = dp_curr;
                prev_max_h = curr_max_h;
        }

        max_computed = n;
        return ans_arr[n];
}
