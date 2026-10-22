/*
 * A300368  Number of nX4 0..1 arrays with every element equal to 2 or 3
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 47, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A300368: DP over rows.  Each configuration of n×4 binary
   arrays with the upper‑left element 0 is counted iff every cell
   has exactly 2 or 3 neighbouring cells (horizontally, vertically,
   or along the antidiagonal) that are equal to it.  The algorithm
   uses a 17×17 transition table for all pairs of 4‑bit rows
   (16 real row patterns and one sentinel value 16 for "none"). */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define ROWS 17            /* 0..15 real rows, 16 = none */
#define NONE 16

/* Return true iff the middle row 'curr' satisfies the
   constraint with neighbours 'prev' and 'next'.
   'prev' or 'next' may be NONE, meaning the corresponding row
   is absent.  Neighbour directions are:
   west, east, north, south, north‑east, south‑west. */
static inline bool ok_transition(uint8_t prev, uint8_t curr, uint8_t next)
{
    bool has_prev = (prev != NONE);
    bool has_next = (next != NONE);
    for (int j = 0; j < 4; ++j) {
        int cnt = 0;
        unsigned int curbit = (curr >> j) & 1U;
        /* Vertical neighbours */
        if (has_prev && ((prev >> j) & 1U) == curbit) ++cnt;   /* north */
        if (has_next && ((next >> j) & 1U) == curbit) ++cnt;   /* south */
        /* Horizontal neighbours */
        if (j > 0 && ((curr >> (j - 1)) & 1U) == curbit) ++cnt; /* west */
        if (j < 3 && ((curr >> (j + 1)) & 1U) == curbit) ++cnt; /* east */
        /* Antidiagonal neighbours */
        if (has_prev && j < 3 && ((prev >> (j + 1)) & 1U) == curbit) ++cnt;   /* north‑east */
        if (has_next && j > 0 && ((next >> (j - 1)) & 1U) == curbit) ++cnt;   /* south‑west */
        if (cnt < 2 || cnt > 3) return false;
    }
    return true;
}

/* Transition table: trans[p][c][n] == true if row c is valid
   between p and n.  The table is filled lazily on first call. */
static bool trans[ROWS][ROWS][ROWS];
static bool trans_ok = false;
static void init_trans(void)
{
    if (trans_ok) return;
    for (uint8_t p = 0; p < ROWS; ++p)
        for (uint8_t c = 0; c < ROWS; ++c)
            for (uint8_t n = 0; n < ROWS; ++n)
                trans[p][c][n] = ok_transition(p, c, n);
    trans_ok = true;
}

/* Main function required by the interface */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init_trans();

    static seq_t dp[ROWS][ROWS];
    static seq_t nxt[ROWS][ROWS];
    /* clear dp */
    memset(dp, 0, sizeof(dp));

    /* First row: only rows with upper‑left element 0 are allowed,
       and there is no previous row (NONE). */
    for (uint8_t r = 0; r < 16; ++r)
        if ((r & 1U) == 0)                 /* upper‑left bit is 0 */
            dp[NONE][r] = 1;

    if (n == 1) {
        seq_t res = 0;
        for (uint8_t p = 0; p < ROWS; ++p)
            for (uint8_t c = 0; c < ROWS; ++c)
                if (dp[p][c] && trans[p][c][NONE])
                    res += dp[p][c];
        return res;
    }

    /* Process rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        memset(nxt, 0, sizeof(nxt));
        for (uint8_t p = 0; p < ROWS; ++p) {
            for (uint8_t c = 0; c < ROWS; ++c) {
                seq_t w = dp[p][c];
                if (!w) continue;
                for (uint8_t nrow = 0; nrow < 16; ++nrow) {  /* next row cannot be NONE */
                    if (trans[p][c][nrow])
                        nxt[c][nrow] += w;
                }
            }
        }
        /* copy nxt back to dp */
        memcpy(dp, nxt, sizeof(dp));
    }

    /* Final row: its lower neighbours are missing (NONE). */
    seq_t result = 0;
    for (uint8_t p = 0; p < ROWS; ++p)
        for (uint8_t c = 0; c < ROWS; ++c)
            if (dp[p][c] && trans[p][c][NONE])
                result += dp[p][c];

    return result;
}
