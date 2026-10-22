/*
 * A300462  Number of nX5 0..1 arrays with every element equal to 2, 3 or 4
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays with the upper left element 0
 *            and each cell having 2, 3 or 4 neighbours with equal value,
 *            counting horizontal, vertical and anti‑diagonal (up‑right /
 *            down‑left) adjacency. Computed by dynamic programming over rows
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* 5 columns, 32 of them */
#define COLS      5
#define MASKS     (1U << COLS)
#define SENTINEL  32   /* sentinel index for “non‑existent” row */

/* Helper to test validity of a row given its previous and next rows.
   prev or next being SENTINEL means the row does not exist. */
static bool valid_row(int prev, int curr, int next)
{
    for (int c = 0; c < COLS; ++c) {
        int bit = (curr >> c) & 1;
        int same = 0;

        /* left */
        if (c > 0 && ((curr >> (c - 1)) & 1) == bit) ++same;
        /* right */
        if (c < COLS - 1 && ((curr >> (c + 1)) & 1) == bit) ++same;

        /* up */
        if (prev != SENTINEL) {
            if (((prev >> c) & 1) == bit) ++same;
            /* up-right */
            if (c < COLS - 1 && ((prev >> (c + 1)) & 1) == bit) ++same;
        }

        /* down */
        if (next != SENTINEL) {
            if (((next >> c) & 1) == bit) ++same;
            /* down-left */
            if (c > 0 && ((next >> (c - 1)) & 1) == bit) ++same;
        }

        if (same < 2 || same > 4) return false;
    }
    return true;
}

/* Precomputed transition table: allow[prev*MASKS + curr][next] == true
   iff row 'curr' satisfies its constraint with rows 'prev' and 'next'. */
static bool allow[MASKS * MASKS][MASKS];

/* Build the transition table once. */
static void build_allow(void)
{
    for (int prev = 0; prev < MASKS; ++prev) {
        for (int curr = 0; curr < MASKS; ++curr) {
            int idx = prev * MASKS + curr;
            for (int next = 0; next < MASKS; ++next) {
                allow[idx][next] = valid_row(prev, curr, next);
            }
        }
    }
    /* For sentinel support: if prev or next is SENTINEL,
       we compute separately when needed. */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1, but safeguard */

    /* Build transition table on first call */
    static bool built = false;
    if (!built) { build_allow(); built = true; }

    /* dp states: index = prev * MASKS + curr ; count of ways */
    seq_t dp[MASKS * MASKS] = {0};
    seq_t newdp[MASKS * MASKS] = {0};

    /* n == 1 special case: only one row, no next row */
    if (n == 1) {
        seq_t ans = 0;
        for (int m = 0; m < MASKS; ++m) {
            if ((m & 1) == 0) { /* top‑left zero fixed */
                if (valid_row(SENTINEL, m, SENTINEL))
                    ans += 1;
            }
        }
        return ans;
    }

    /* Initialize for first two rows */
    for (int m0 = 0; m0 < MASKS; ++m0) {
        if ((m0 & 1) != 0) continue; /* top-left must be 0 */
        for (int m1 = 0; m1 < MASKS; ++m1) {
            if (valid_row(SENTINEL, m0, m1)) {
                dp[(m0 * MASKS) + m1] = 1;
            }
        }
    }

    /* Process rows 2 .. n-1 */
    for (idx_t r = 2; r < n; ++r) {
        for (size_t i = 0; i < sizeof(dp)/sizeof(dp[0]); ++i) newdp[i] = 0;
        for (int prev = 0; prev < MASKS; ++prev) {
            for (int curr = 0; curr < MASKS; ++curr) {
                seq_t cnt = dp[(prev * MASKS) + curr];
                if (cnt == 0) continue;
                for (int next = 0; next < MASKS; ++next) {
                    if (allow[(prev * MASKS) + curr][next]) {
                        newdp[(curr * MASKS) + next] += cnt;
                    }
                }
            }
        }
        for (size_t i = 0; i < sizeof(dp)/sizeof(dp[0]); ++i) dp[i] = newdp[i];
    }

    /* Final row (last row) no next row */
    seq_t ans = 0;
    for (int prev = 0; prev < MASKS; ++prev) {
        for (int curr = 0; curr < MASKS; ++curr) {
            seq_t cnt = dp[(prev * MASKS) + curr];
            if (cnt == 0) continue;
            if (valid_row(prev, curr, SENTINEL)) {
                ans += cnt;
            }
        }
    }
    return ans;
}
