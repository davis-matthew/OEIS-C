/*
 * A302364  Number of nX5 0..1 arrays with every element equal to 0, 2, 3 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of n×5 zero‑one arrays whose every element has
 * 0, 2, 3 or 4 adjacent equal elements (horizontally, diagonally or
 * antidiagonally).  The upper‑left element is fixed to 0.
 * Implementation uses dynamic programming on pairs of consecutive rows.
 * The function is O(n·32²) and works for any n≥1. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Count of adjacent equal elements for a single row.
 *   prev  : previous row (or 0 if none)
 *   curr  : current row
 *   has_prev : true if prev is valid (not the first row)
 *   has_next : true if a next row exists
 *   next  : next row (or 0 if none)
 * Return true if all cells of curr satisfy the 0,2,3,4 rule.  */
static bool row_ok(uint8_t prev, uint8_t curr, bool has_prev,
                   bool has_next, uint8_t next)
{
    for (int j = 0; j < 5; ++j) {
        int v   = (curr >> j) & 1;
        int horiz = 0, diag = 0, antidiag = 0;

        /* horizontal neighbours */
        if (j > 0)   horiz += (((curr >> (j - 1)) & 1) == v);
        if (j < 4)   horiz += (((curr >> (j + 1)) & 1) == v);

        /* diagonal neighbours: up‑left / down‑right */
        if (has_prev && j > 0)   diag += (((prev >> (j - 1)) & 1) == v);
        if (has_next && j < 4)   diag += (((next >> (j + 1)) & 1) == v);

        /* antidiagonal neighbours: up‑right / down‑left */
        if (has_prev && j < 4)   antidiag += (((prev >> (j + 1)) & 1) == v);
        if (has_next && j > 0)   antidiag += (((next >> (j - 1)) & 1) == v);

        int total = horiz + diag + antidiag;
        if (!(total == 0 || total == 2 || total == 3 || total == 4))
            return false;
    }
    return true;
}

/* Pre‑compute the transition possibilities between a pair of rows. */
static uint32_t trans[32][32];
static bool trans_ready = false;

static void build_trans(void)
{
    if (trans_ready) return;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            uint32_t mask = 0;
            for (int next = 0; next < 32; ++next) {
                if (row_ok((uint8_t)prev, (uint8_t)curr, true, true,
                           (uint8_t)next))
                    mask |= (1U << next);
            }
            trans[prev][curr] = mask;
        }
    }
    trans_ready = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */

    build_trans();

    /* Handle the single‑row case directly. */
    if (n == 1) {
        seq_t ans = 0;
        for (int row = 0; row < 32; ++row) {
            if (row & 1) continue;   /* top‑left must be 0 */
            if (row_ok(0, (uint8_t)row, false, false, 0))
                ans += (seq_t)1;
        }
        return ans;
    }

    /* DP arrays for pairs of rows. */
    static seq_t dp[32][32];
    static seq_t ndp[32][32];
    seq_t (*cur)[32]  = dp;
    seq_t (*next)[32] = ndp;

    /* Initialise all to zero. */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            cur[i][j] = 0;

    /* First two rows: choose r0 (top‑left 0) and r1 such that r0 is valid. */
    for (int r0 = 0; r0 < 32; ++r0) {
        if (r0 & 1) continue;              /* top‑left requires 0 */
        for (int r1 = 0; r1 < 32; ++r1) {
            if (row_ok(0, (uint8_t)r0, false, true, (uint8_t)r1))
                cur[r0][r1] += (seq_t)1;
        }
    }

    /* Transition for rows 3 … n. */
    for (idx_t step = 2; step < n; ++step) {
        /* Clear next array. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                next[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t ways = cur[prev][curr];
                if (!ways) continue;
                uint32_t mask = trans[prev][curr];
                for (int nxt = 0; nxt < 32; ++nxt) {
                    if (mask & (1U << nxt))
                        next[curr][nxt] += ways;
                }
            }
        }
        /* Swap current and next. */
        seq_t (*tmp)[32] = cur;
        cur = next;
        next = tmp;
    }

    /* Final row must satisfy constraints (no row below). */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            seq_t ways = cur[prev][curr];
            if (!ways) continue;
            if (row_ok((uint8_t)prev, (uint8_t)curr, true, false, 0))
                ans += ways;
        }
    }
    return ans;
}
