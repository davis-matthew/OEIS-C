/*
 * A298551  Number of nX5 0..1 arrays with every element equal to 0, 2, 3, 5
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A298551: number of n×5 0–1 arrays whose king‑move neighbourhood
   count of equal entries is in {0,2,3,5,7}.  The upper‑left cell is fixed
   to 0.  The function uses a row‑by‑row dynamic programming over
   5‑bit row masks (32 states). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Allowed neighbour counts: 0,2,3,5,7 are true, all others false. */
static const bool allowed[9] = {
    true,   /* 0 */
    false,  /* 1 */
    true,   /* 2 */
    true,   /* 3 */
    false,  /* 4 */
    true,   /* 5 */
    false,  /* 6 */
    true,   /* 7 */
    false   /* 8 */
};

/* Check whether a triple of rows (prev, curr, next) satisfies the local
   rule for all cells of 'curr'.
   prev_exists, next_exists indicate whether the corresponding rows
   are present (top or bottom boundary).  Rows are 5‑bit masks. */
static bool valid_triple(int prev, int curr, int next,
                         bool prev_exists, bool next_exists)
{
    for (int j = 0; j < 5; ++j) {
        int val = (curr >> j) & 1;
        int cnt = 0;

        /* neighbours in the previous row (north, north‑west, north‑east) */
        if (prev_exists) {
            for (int d = -1; d <= 1; ++d) {
                int k = j + d;
                if (k >= 0 && k < 5 && ((prev >> k) & 1) == val) ++cnt;
            }
        }

        /* left/right neighbours in the current row (west, east) */
        for (int d = -1; d <= 1; d += 2) {
            int k = j + d;
            if (k >= 0 && k < 5 && ((curr >> k) & 1) == val) ++cnt;
        }

        /* neighbours in the next row (south, south‑west, south‑east) */
        if (next_exists) {
            for (int d = -1; d <= 1; ++d) {
                int k = j + d;
                if (k >= 0 && k < 5 && ((next >> k) & 1) == val) ++cnt;
            }
        }

        if (cnt > 8 || !allowed[cnt]) return false;
    }
    return true;
}

/* Transition table: for each pair (prev,curr) we store all possible next
   rows that keep the current row valid.  1024 = 32×32 states. */
static int trans_next[1024][32];
static int trans_count[1024] = {0};

static void init_transitions(void)
{
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            int idx = prev * 32 + curr;
            int cnt = 0;
            for (int next = 0; next < 32; ++next) {
                if (valid_triple(prev, curr, next, true, true)) {
                    trans_next[idx][cnt++] = next;
                }
            }
            trans_count[idx] = cnt;
        }
    }
}

seq_t a(idx_t n)
{
    static bool ready = false;
    if (!ready) { init_transitions(); ready = true; }

    /* Handle n == 1 specially. */
    if (n == 1) {
        seq_t res = 0;
        for (int mask = 0; mask < 32; ++mask) {
            if (mask & 1) continue;                       /* upper‑left zero */
            if (valid_triple(-1, mask, -1, false, false)) res += 1;
        }
        return res;
    }

    /* DP arrays: counts for (prev,curr) pairs. */
    seq_t *dp_cur = (seq_t*)calloc(1024, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)calloc(1024, sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        free(dp_cur); free(dp_next);
        return 0;
    }

    /* Initialise first two rows. */
    for (int row0 = 0; row0 < 32; ++row0) {
        if (row0 & 1) continue;                         /* upper‑left zero */
        for (int row1 = 0; row1 < 32; ++row1) {
            if (valid_triple(-1, row0, row1, false, true))
                dp_cur[row0 * 32 + row1] = 1;
        }
    }

    /* Build rows 2 .. n−1. */
    for (idx_t step = 2; step < n; ++step) {
        for (int i = 0; i < 1024; ++i) dp_next[i] = 0;

        for (int s = 0; s < 1024; ++s) {
            seq_t ways = dp_cur[s];
            if (!ways) continue;
            int curr = s % 32;
            int cnt = trans_count[s];
            for (int t = 0; t < cnt; ++t) {
                int next = trans_next[s][t];
                int newidx = curr * 32 + next;
                dp_next[newidx] += ways;
            }
        }
        seq_t *tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;
    }

    /* Final step: validate last row (no next row). */
    seq_t result = 0;
    for (int s = 0; s < 1024; ++s) {
        seq_t ways = dp_cur[s];
        if (!ways) continue;
        int prev = s / 32;
        int curr = s % 32;
        if (valid_triple(prev, curr, -1, true, false))
            result += ways;
    }

    free(dp_cur); free(dp_next);
    return result;
}
