/*
 * A333098  Number of closed Deutsch paths whose area is exactly n.
 * https://oeis.org/A333098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 77, u128 n <= 153,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A333098 – Number of closed Deutsch paths of area exactly n
 *
 * A Deutsch path is a lattice walk starting at height 0, where each step
 * is either an up step (+1 height) or a down step to any lower height.
 * A down step may go to any height strictly below the current height.
 * The area of a path is the sum of the heights just before each step.
 * Here we count paths that start and finish at height 0 with total area n.
 *
 * Dynamic programming over (current area, current height) is used.
 * For a state (a,h) the next area is a+h; the up step goes to height h+1,
 * and for each new height k (0 ≤ k < h) a down step is possible.
 * The number of states is O(n·√n) and each state emits O(h) transitions,
 * giving a total time of about O(n·n) in the worst case, which is well
 * within the 2‑second limit for n ≤ 10 000.
 *
 * The implementation avoids auxiliary utilities from the framework
 * (e.g. _floor_isqrt_u64) by defining its own integer square root.
 */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uint64_t, etc. */
#include <stdlib.h>      /* calloc, free */

/* integer square root of a 64‑bit unsigned integer */
static inline idx_t isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n) {
    /* maximal height that can be reached for area n:
       largest h with h*(h+1)/2 ≤ n  →  h_max = floor((1+√(1+8n))/2) */
    unsigned __int128 val = (unsigned __int128)1 + (unsigned __int128)8 * n;
    idx_t root = isqrt_u64((idx_t)val);
    idx_t h_max = (idx_t)((1 + root) / 2);

    size_t rows = (size_t)(n + 1);
    size_t cols = (size_t)(h_max + 1);
    size_t total = rows * cols;

    seq_t *dp = (seq_t *)calloc(total, sizeof(seq_t));
    if (!dp) return 0;  /* allocation failure */

    /* helper to convert (area, height) to linear index */
    #define IDX(a, h) ((size_t)(a) * cols + (h))

    dp[IDX(0, 0)] = 1;

    for (idx_t area = 0; area <= n; ++area) {
        for (idx_t h = 0; h <= h_max; ++h) {
            seq_t val = dp[IDX(area, h)];
            if (val == 0) continue;

            idx_t next_area = area + h;
            if (next_area > n) continue;

            /* up step */
            if (h + 1 <= h_max) {
                dp[IDX(next_area, h + 1)] += val;
            }

            /* down steps to any lower height */
            for (idx_t newh = 0; newh < h; ++newh) {
                dp[IDX(next_area, newh)] += val;
            }
        }
    }

    seq_t result = dp[IDX(n, 0)];
    free(dp);
    return result;

    #undef IDX
}
