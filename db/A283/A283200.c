/*
 * A283200  Number of nX5 0..1 arrays with no 1 equal to more than two of its
 *          horizontal and vertical neighbors, with the exception of exactly
 *          one element.
 * https://oeis.org/A283200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays in which exactly one cell may
 *            have more than two 1‑neighboring cells (horizontal or vertical);
 *            all other 1s must have at most two such neighbors
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   count_row_violations
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/count_row_violations.h"

/* Helper structure to store admissible transitions for a given triple
   (a,b) -> c where a=(prev_prev row), b=(prev row), c=(current row). */
static struct Transition {
    uint8_t v0[32];      /* c values yielding 0 violations in row b */
    uint8_t sz0;          /* number of such values */
    uint8_t v1[32];      /* c values yielding 1 violation in row b */
    uint8_t sz1;          /* number of such values */
} trans[32][32];

static int transitions_precomputed = 0;

static void precompute_transitions(void)
{
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            struct Transition *t = &trans[a][b];
            t->sz0 = t->sz1 = 0;
            for (int c = 0; c < 32; ++c) {
                uint8_t v = count_row_violations((uint8_t)a, (uint8_t)b, (uint8_t)c);
                if (v == 0) {
                    t->v0[t->sz0++] = (uint8_t)c;
                } else if (v == 1) {
                    t->v1[t->sz1++] = (uint8_t)c;
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not part of the sequence, but harmless */

    /* Compute transition tables once. */
    if (!transitions_precomputed) {
        precompute_transitions();
        transitions_precomputed = 1;
    }

    /* DP state: dp[idx][viol] where idx = prev_prev_row*32 + prev_row. */
    static seq_t dpCurr[1024][2];
    static seq_t dpNext[1024][2];

    /* Clear current DP. */
    for (int i = 0; i < 1024; ++i) {
        dpCurr[i][0] = dpCurr[i][1] = 0;
    }

    /* Initialise: prev_prev = 0 (top boundary), prev = first real row. */
    for (int first = 0; first < 32; ++first) {
        dpCurr[first][0] = 1;   /* one way to start with this row, no violations yet */
    }

    /* Process rows 2 … n. */
    for (idx_t step = 1; step < n; ++step) {
        /* Clear next DP. */
        for (int i = 0; i < 1024; ++i) {
            dpNext[i][0] = dpNext[i][1] = 0;
        }

        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                int idx = a * 32 + b;
                seq_t val0 = dpCurr[idx][0];
                seq_t val1 = dpCurr[idx][1];
                if (!val0 && !val1) continue;

                const struct Transition *t = &trans[a][b];

                /* Transitions with 0 violations in the middle row. */
                for (int k = 0; k < t->sz0; ++k) {
                    int c = t->v0[k];
                    int idxNext = b * 32 + c;
                    if (val0) dpNext[idxNext][0] += val0;
                    if (val1) dpNext[idxNext][1] += val1; /* violation count unchanged */
                }

                /* Transitions with 1 violation in the middle row – only allowed
                   if no violation has been seen before. */
                if (val0) {
                    for (int k = 0; k < t->sz1; ++k) {
                        int c = t->v1[k];
                        int idxNext = b * 32 + c;
                        dpNext[idxNext][1] += val0;
                    }
                }
            }
        }

        /* Swap dpCurr and dpNext. */
        for (int i = 0; i < 1024; ++i) {
            dpCurr[i][0] = dpNext[i][0];
            dpCurr[i][1] = dpNext[i][1];
        }
    }

    /* Final evaluation with dummy bottom row (all zeros). */
    seq_t ans = 0;
    const int dummy = 0;
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            int idx = a * 32 + b;
            seq_t val0 = dpCurr[idx][0];
            seq_t val1 = dpCurr[idx][1];
            if (!val0 && !val1) continue;

            /* Count violations contributed by the last row b. */
            uint8_t v = count_row_violations((uint8_t)a, (uint8_t)b, (uint8_t)dummy);
            if (v == 0) {
                /* Only arrays that already had one violation survive. */
                if (val1) ans += val1;
            } else if (v == 1) {
                /* Only arrays that had no violation yet become valid. */
                if (val0) ans += val0;
            }
            /* v > 1 : invalid configuration, ignore. */
        }
    }

    return ans;
}
