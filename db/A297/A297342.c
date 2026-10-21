/*
 * A297342  Number of 5Xn 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A297342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of 5×n binary arrays with each 1 having 0 or 2 *
 *            neighbours among left, right, up‑left, down‑right. * Computed by
 *            dynamic programming over column masks
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* 5 rows → each column fits in 5 bits → 32 masks. */
#define WIDTH   5
#define MASKS   (1 << WIDTH)          /* 32 */
#define STATES  (MASKS * MASKS)       /* 1024 */

/* Return 1 if the 1s in mask 'curr' satisfy the neighbour rule
 * when the left column is 'prev' and the right column is 'next'.
 * Only the four neighbours left, right, up‑left, down‑right are
 * considered. */
static int validTriple(int prev, int curr, int next)
{
    for (int r = 0; r < WIDTH; ++r) {
        if (curr & (1u << r)) {
            int cnt = 0;
            if (prev & (1u << r))                 cnt++;  /* left   */
            if (next & (1u << r))                 cnt++;  /* right  */
            if (r > 0 && (prev & (1u << (r - 1))))cnt++;  /* up‑left  */
            if (r + 1 < WIDTH && (next & (1u << (r + 1))))cnt++; /* down‑right */
            if (cnt != 0 && cnt != 2) return 0;
        }
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1 – unused */

    /* Pre‑compute, once, the set of valid next masks for each pair
     * (prev, curr).  The result is stored as a bitset. */
    static uint32_t valid_next[STATES] = {0};
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        for (int prev = 0; prev < MASKS; ++prev) {
            for (int curr = 0; curr < MASKS; ++curr) {
                uint32_t bits = 0;
                for (int next = 0; next < MASKS; ++next) {
                    if (validTriple(prev, curr, next))
                        bits |= 1u << next;
                }
                valid_next[(prev << WIDTH) | curr] = bits;
            }
        }
    }

    /* Dynamic programming tables.  Both tables have STATES entries. */
    static seq_t dp[STATES];
    static seq_t newdp[STATES];

    /* Initialise dp for the first column: prev = 0 (dummy), curr = mask. */
    for (int i = 0; i < STATES; ++i) dp[i] = 0;
    for (int mask = 0; mask < MASKS; ++mask)
        dp[mask] = 1;                      /* index = (0 << WIDTH) | mask */

    /* Process the remaining columns.  Each step validates the
     * previous column and chooses the next column. */
    for (idx_t step = 0; step + 1 < n; ++step) {
        for (int i = 0; i < STATES; ++i) newdp[i] = 0;
        for (int idx = 0; idx < STATES; ++idx) {
            seq_t val = dp[idx];
            if (val == 0) continue;
            int curr = idx & (MASKS - 1);          /* curr mask */
            uint32_t bits = valid_next[idx];
            /* iterate over all next masks allowed for this (prev,curr) pair */
            while (bits) {
                /* GCC / Clang builtin: get least‑significant set bit */
                int next = __builtin_ctz(bits);
                bits &= bits - 1;
                int new_idx = (curr << WIDTH) | next;
                newdp[new_idx] += val;
            }
        }
        /* swap dp and newdp for next iteration */
        for (int i = 0; i < STATES; ++i) dp[i] = newdp[i];
    }

    /* Final validation: the last column has right neighbour 0. */
    seq_t ans = 0;
    for (int idx = 0; idx < STATES; ++idx) {
        seq_t val = dp[idx];
        if (val == 0) continue;
        /* bit 0 of valid_next[idx] corresponds to next = 0. */
        if (valid_next[idx] & 1u)
            ans += val;
    }

    return ans;
}
