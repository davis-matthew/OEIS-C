/*
 * A250596  Number of (n+2) X (3+2) 0..2 arrays with nondecreasing sum of
 *          every three consecutive values in every row and column.
 * https://oeis.org/A250596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = w(n)^2 * v(n) where w(n) = number of arrays of size (n+2)
 *            × 2 with values 0–2, satisfying a0 ≤ a1 per row and vertical
 *            nondecreasing by 3 (i.e., a[row] ≥ a[row‑3]). v(n) = number of
 *            arrays of size (n+2) × 1 with values 0–2 and vertical
 *            nondecreasing by 3
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 15, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* All admissible (a0,a1) pairs with a0 ≤ a1. */
    const int PAIRS[6][2] = {
        {0,0},{0,1},{0,2},{1,1},{1,2},{2,2}
    };

    /* Number of rows. */
    const idx_t R = (idx_t)n + 2;

    /* ---------- compute v(n) ---------- */
    seq_t v = 1;
    for (int r = 0; r < 3; ++r) {
        if (R > (idx_t)r) {
            idx_t L = ((R - 1 - (idx_t)r) / 3) + 1;      /* length of this residue class */
            seq_t temp = (seq_t)(L + 1) * (seq_t)(L + 2) / (seq_t)2; /* C(L+2,2) */
            v *= temp;
        }
    }

    /* ---------- compute w(n) via DP ---------- */
    /* State: pair of triples (a0[3], a1[3]), each encoded as 0..26. */
    const size_t STATE_COUNT = 729;                     /* 27 × 27 */
    seq_t dp[STATE_COUNT];
    seq_t next[STATE_COUNT];

    /* Initialise to zero. */
    for (size_t i = 0; i < STATE_COUNT; ++i) dp[i] = (seq_t)0;

    /* Enumerate the first three rows (no vertical constraints yet). */
    for (int i0 = 0; i0 < 6; ++i0) {
        int a0_0 = PAIRS[i0][0];
        int a1_0 = PAIRS[i0][1];
        for (int i1 = 0; i1 < 6; ++i1) {
            int a0_1 = PAIRS[i1][0];
            int a1_1 = PAIRS[i1][1];
            for (int i2 = 0; i2 < 6; ++i2) {
                int a0_2 = PAIRS[i2][0];
                int a1_2 = PAIRS[i2][1];
                size_t triple0 = (size_t)a0_0 * 9 + (size_t)a0_1 * 3 + (size_t)a0_2;
                size_t triple1 = (size_t)a1_0 * 9 + (size_t)a1_1 * 3 + (size_t)a1_2;
                size_t idx = triple0 * 27 + triple1;
                dp[idx] += (seq_t)1;
            }
        }
    }

    /* Process subsequent rows (if any). */
    for (idx_t row = 3; row < R; ++row) {
        for (size_t i = 0; i < STATE_COUNT; ++i) next[i] = (seq_t)0;

        for (size_t s = 0; s < STATE_COUNT; ++s) {
            seq_t ways = dp[s];
            if (ways == (seq_t)0) continue;

            size_t t0 = s / 27;
            int a0_0 = (int)(t0 / 9);
            int a0_1 = (int)((t0 / 3) % 3);
            int a0_2 = (int)(t0 % 3);

            size_t t1 = s % 27;
            int a1_0 = (int)(t1 / 9);
            int a1_1 = (int)((t1 / 3) % 3);
            int a1_2 = (int)(t1 % 3);

            for (int p = 0; p < 6; ++p) {
                int new0 = PAIRS[p][0];
                int new1 = PAIRS[p][1];

                if (new0 < a0_0 || new1 < a1_0) continue;  /* enforce vertical constraint */

                size_t new_t0 = (size_t)a0_1 * 9 + (size_t)a0_2 * 3 + (size_t)new0;
                size_t new_t1 = (size_t)a1_1 * 9 + (size_t)a1_2 * 3 + (size_t)new1;
                size_t new_idx = new_t0 * 27 + new_t1;
                next[new_idx] += ways;
            }
        }

        /* Swap dp and next. */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp[i] = next[i];
    }

    /* Sum all terminal states to obtain w(n). */
    seq_t w = 0;
    for (size_t i = 0; i < STATE_COUNT; ++i) w += dp[i];

    /* Final result: a(n) = w^2 * v. */
    seq_t ans = w * w;
    ans *= v;
    return ans;
}
