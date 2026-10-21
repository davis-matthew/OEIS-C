/*
 * A298778  Number of n X 4 0..1 arrays with every element equal to 0, 3, 4, 5
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 40, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A298778: number of n×4 binary arrays (n≥1) with upper‑left
 * element 0 such that for every cell the number of king‑move adjacent
 * cells having the same value is in {0,3,4,5,8}.
 *
 * The implementation uses a dynamic‑programming approach over rows.
 * For each pair of consecutive rows we pre‑compute whether the upper
 * row is valid given the middle and lower rows.  A 3‑dimensional
 * table holds these Boolean values, with the sentinel value 16
 * representing a missing neighbour.  This table is built once.
 *
 * Complexity:  O(17·16·17·n) ≈ O(4368·n) operations, fast for
 * n up to 10 000.  Memory usage is constant.  No external libraries
 * are required.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define NONE 16                           /* sentinel for missing row */

/* Allowed numbers of equal king‑move neighbours (0,3,4,5,8). */
static const bool ALLOWED[9] = {
    true,  /* 0 */
    false, /* 1 */
    false, /* 2 */
    true,  /* 3 */
    true,  /* 4 */
    true,  /* 5 */
    false, /* 6 */
    false, /* 7 */
    true   /* 8 */
};

/* VALID[prev][curr][next] is true iff row `curr` is valid when its
 * upper neighbour is `prev` and lower neighbour is `next`.
 * Dimensions: prev ∈ {0..16}, curr ∈ {0..15}, next ∈ {0..16}. */
static bool VALID[17][16][17];
static bool VALID_COMPUTED = false;

/* Test whether a row is valid given the two neighbour rows. */
static inline bool row_is_valid(int prev, int curr, int next)
{
    for (int c = 0; c < 4; ++c) {
        int val   = (curr >> c) & 1;
        int equal = 0;

        /* Horizontal neighbours in the same row. */
        if (c > 0     && ((curr >> (c - 1)) & 1) == val) ++equal; /* left  */
        if (c < 3     && ((curr >> (c + 1)) & 1) == val) ++equal; /* right */

        /* Vertical neighbours from the upper row. */
        if (prev != NONE) {
            if (((prev >> c) & 1) == val)            ++equal; /* up    */
            if (c > 0     && ((prev >> (c - 1)) & 1) == val) ++equal; /* up‑left */
            if (c < 3     && ((prev >> (c + 1)) & 1) == val) ++equal; /* up‑right */
        }

        /* Vertical neighbours from the lower row. */
        if (next != NONE) {
            if (((next >> c) & 1) == val)            ++equal; /* down    */
            if (c > 0     && ((next >> (c - 1)) & 1) == val) ++equal; /* down‑left */
            if (c < 3     && ((next >> (c + 1)) & 1) == val) ++equal; /* down‑right */
        }

        if (!ALLOWED[equal]) return false;
    }
    return true;
}

/* Initialise the VALID table once. */
static void init_valid_table(void)
{
    if (VALID_COMPUTED) return;
    for (int p = 0; p <= NONE; ++p)
        for (int c = 0; c < 16; ++c)
            for (int n = 0; n <= NONE; ++n)
                VALID[p][c][n] = row_is_valid(p, c, n);
    VALID_COMPUTED = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    init_valid_table();

    static seq_t dp[17][16];
    static seq_t ndp[17][16];

    /* Clear dp. */
    for (int p = 0; p <= NONE; ++p)
        for (int c = 0; c < 16; ++c)
            dp[p][c] = 0;

    /* First row: top neighbour missing, leftmost cell must be 0. */
    for (int r = 0; r < 16; ++r)
        if ((r & 1) == 0)                  /* bit 0 == 0 (leftmost column) */
            dp[NONE][r] = 1;

    /* If only one row, finish. */
    if (n == 1) {
        seq_t res = 0;
        for (int prev = 0; prev <= NONE; ++prev)
            for (int curr = 0; curr < 16; ++curr)
                if (dp[prev][curr] && VALID[prev][curr][NONE])
                    res += dp[prev][curr];
        return res;
    }

    /* Build rows 2 .. n. */
    for (idx_t step = 1; step < n; ++step) {
        /* Clear ndp. */
        for (int p = 0; p <= NONE; ++p)
            for (int c = 0; c < 16; ++c)
                ndp[p][c] = 0;

        for (int prev = 0; prev <= NONE; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t curCnt = dp[prev][curr];
                if (!curCnt) continue;

                for (int next = 0; next < 16; ++next) {
                    if (!VALID[prev][curr][next]) continue;
                    ndp[curr][next] += curCnt;
                }
            }
        }

        /* Swap dp and ndp. */
        for (int p = 0; p <= NONE; ++p)
            for (int c = 0; c < 16; ++c)
                dp[p][c] = ndp[p][c];
    }

    /* Final validation of the bottom row (no lower neighbour). */
    seq_t result = 0;
    for (int prev = 0; prev <= NONE; ++prev)
        for (int curr = 0; curr < 16; ++curr)
            if (dp[prev][curr] && VALID[prev][curr][NONE])
                result += dp[prev][curr];

    return result;
}
