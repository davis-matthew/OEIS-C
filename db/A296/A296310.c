/*
 * A296310  Number of nX5 0..1 arrays with each 1 adjacent to 3 or 6 king-move
 *          neighboring 1s.
 * https://oeis.org/A296310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 93,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 204 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296310: Number of n×5 binary arrays in which each 1 has 3 or 6 king‑move
   neighboring 1s.  The array is counted for n ≥ 1. */
#include <stdbool.h>

static bool validTriple(uint32_t above, uint32_t mid, uint32_t below) {
    for (int c = 0; c < 5; ++c) {
        if (((mid >> c) & 1u) == 0) continue;          /* only 1‑cells are restricted */
        int cnt = 0;
        /* above row */
        if (c - 1 >= 0 && ((above >> (c - 1)) & 1u)) ++cnt;
        if ((above >> c) & 1u) ++cnt;
        if (c + 1 < 5 && ((above >> (c + 1)) & 1u)) ++cnt;
        /* mid row */
        if (c - 1 >= 0 && ((mid >> (c - 1)) & 1u)) ++cnt;
        if (c + 1 < 5 && ((mid >> (c + 1)) & 1u)) ++cnt;
        /* below row */
        if (c - 1 >= 0 && ((below >> (c - 1)) & 1u)) ++cnt;
        if ((below >> c) & 1u) ++cnt;
        if (c + 1 < 5 && ((below >> (c + 1)) & 1u)) ++cnt;
        if (cnt != 3 && cnt != 6) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Precomputed transition masks: for each pair (above, middle) the bits set in
       mask represent allowed new rows (below) such that middle satisfies the rule. */
    static uint32_t next_mask[32][32];
    static bool init = false;
    if (!init) {
        for (uint32_t ab = 0; ab < 32; ++ab) {
            for (uint32_t mid = 0; mid < 32; ++mid) {
                uint32_t mask = 0;
                for (uint32_t below = 0; below < 32; ++below) {
                    if (validTriple(ab, mid, below))
                        mask |= (1u << below);
                }
                next_mask[ab][mid] = mask;
            }
        }
        init = true;
    }

    /* DP tables: dp[above][middle] counts sequences processed so far, ending with
       rows 'above' and 'middle'. */
    static seq_t dp[32][32];
    static seq_t dp_next[32][32];
    for (uint32_t i = 0; i < 32; ++i)
        for (uint32_t j = 0; j < 32; ++j)
            dp[i][j] = 0;
    dp[0][0] = 1;  /* start with the two sentinel rows of all zeros */

    /* Process each real row */
    for (idx_t row = 0; row < n; ++row) {
        /* clear next table */
        for (uint32_t i = 0; i < 32; ++i)
            for (uint32_t j = 0; j < 32; ++j)
                dp_next[i][j] = 0;

        for (uint32_t ab = 0; ab < 32; ++ab) {
            for (uint32_t mid = 0; mid < 32; ++mid) {
                seq_t cnt = dp[ab][mid];
                if (cnt == 0) continue;
                uint32_t mask = next_mask[ab][mid];
                for (uint32_t newr = 0; newr < 32; ++newr) {
                    if (mask & (1u << newr)) {
                        dp_next[mid][newr] += cnt;
                    }
                }
            }
        }

        /* swap tables */
        for (uint32_t i = 0; i < 32; ++i)
            for (uint32_t j = 0; j < 32; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Final step: evaluate the last row against the sentinel zero row */
    seq_t result = 0;
    const uint32_t zero = 0;
    for (uint32_t ab = 0; ab < 32; ++ab) {
        for (uint32_t mid = 0; mid < 32; ++mid) {
            seq_t cnt = dp[ab][mid];
            if (cnt == 0) continue;
            if (validTriple(ab, mid, zero))
                result += cnt;
        }
    }
    return result;
}
