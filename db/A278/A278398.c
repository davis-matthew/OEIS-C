/*
 * A278398  Number of meanders (walks starting at the origin and ending at any
 *          altitude >= 0 that may touch but never go below the x-axis) with n
 *          steps from {-3,-2,-1,1,2,3}.
 * https://oeis.org/A278398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = number of length‑n walks with steps from {-3,-2,-1,1,2,3}
 *            that never go below 0. The values are computed by dynamic
 *            programming over the current altitude. The transition matrix is
 *            0‑1 and the algorithm runs in O(n²) time and O(n) memory
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#ifndef __cplusplus
#include <stdlib.h>
#include <string.h>
#endif

seq_t a(idx_t n)
{
    /* n==0: the empty walk */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Maximum height reachable after n steps – 3 per step. */
    const size_t maxH = 3 * (size_t)n;

#ifdef __cplusplus
    /* C++ path – use std::vector for automatic construction/destruction. */
    std::vector<seq_t> dp_prev(maxH + 1, (seq_t)0), dp_next(maxH + 1, (seq_t)0);
    dp_prev[0] = (seq_t)1;

    for (idx_t step = 1; step <= n; ++step) {
        const size_t curMax = 3 * (size_t)step;
        /* zero the part of dp_next that will be used */
        std::fill(dp_next.begin(), dp_next.begin() + curMax + 1, (seq_t)0);
        const size_t prevMax = curMax - 3;           /* 3*(step-1) */
        for (size_t h = 0; h <= prevMax; ++h) {
            seq_t val = dp_prev[h];
            if (val == (seq_t)0) continue;          /* skip zero entries */
            dp_next[h + 1] += val;
            dp_next[h + 2] += val;
            dp_next[h + 3] += val;
            if (h >= 1) dp_next[h - 1] += val;
            if (h >= 2) dp_next[h - 2] += val;
            if (h >= 3) dp_next[h - 3] += val;
        }
        std::swap(dp_prev, dp_next);
    }

    seq_t result = (seq_t)0;
    for (size_t h = 0; h <= maxH; ++h) {
        result += dp_prev[h];
    }
    return result;
#else
    /* C path – use malloc/calloc.  Assumes seq_t is a POD integer type. */
    seq_t *dp_prev = (seq_t*)calloc(maxH + 1, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)calloc(maxH + 1, sizeof(seq_t));
    if (!dp_prev || !dp_next) { /* allocation failed */
        return (seq_t)0;
    }
    dp_prev[0] = (seq_t)1;

    for (idx_t step = 1; step <= n; ++step) {
        const size_t curMax = 3 * (size_t)step;
        /* zero the part of dp_next that will be used */
        memset(dp_next, 0, (curMax + 1) * sizeof(seq_t));
        const size_t prevMax = curMax - 3;           /* 3*(step-1) */
        for (size_t h = 0; h <= prevMax; ++h) {
            seq_t val = dp_prev[h];
            if (val == (seq_t)0) continue;          /* skip zero entries */
            dp_next[h + 1] += val;
            dp_next[h + 2] += val;
            dp_next[h + 3] += val;
            if (h >= 1) dp_next[h - 1] += val;
            if (h >= 2) dp_next[h - 2] += val;
            if (h >= 3) dp_next[h - 3] += val;
        }
        /* swap pointers */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
    }

    seq_t result = (seq_t)0;
    for (size_t h = 0; h <= maxH; ++h) {
        result += dp_prev[h];
    }
    free(dp_prev);
    free(dp_next);
    return result;
#endif
}
