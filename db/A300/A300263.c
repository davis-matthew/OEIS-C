/*
 * A300263  Number of n X 4 0..1 arrays with every element equal to 0, 1, 2,
 *          3, 5, 6, 7 or 8 king-move adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A300263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary matrices with the upper‑left cell
 *            fixed to 0 and with no cell having exactly 4 equal king‑move
 *            adjacent cells
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------- */
/* Helpers for counting equal king‑move neighbors                     */
/* --------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>

#define PAT 16    /* number of 4‑bit rows (0…15) */

/* get the bit at column c (0‑based) in mask */
static inline unsigned bit(unsigned mask, int c)
{
    return (mask >> c) & 1U;
}

/* check a row with no above neighbours, given the row itself and the row below */
static bool row_good_no_above(uint16_t cur, uint16_t bot)
{
    for (int i = 0; i < 4; ++i) {
        unsigned v = bit(cur, i);
        unsigned cnt = 0;
        /* same row */
        if (i > 0 && bit(cur, i - 1) == v) ++cnt;
        if (i < 3 && bit(cur, i + 1) == v) ++cnt;
        /* below row */
        if (i > 0 && bit(bot, i - 1) == v) ++cnt;
        if (bit(bot, i) == v) ++cnt;
        if (i < 3 && bit(bot, i + 1) == v) ++cnt;
        if (cnt == 4) return false;
    }
    return true;
}

/* check a row with no below neighbours, given the row above and the row itself */
static bool row_good_no_below(uint16_t top, uint16_t cur)
{
    for (int i = 0; i < 4; ++i) {
        unsigned v = bit(cur, i);
        unsigned cnt = 0;
        /* above row */
        if (i > 0 && bit(top, i - 1) == v) ++cnt;
        if (bit(top, i) == v) ++cnt;
        if (i < 3 && bit(top, i + 1) == v) ++cnt;
        /* same row */
        if (i > 0 && bit(cur, i - 1) == v) ++cnt;
        if (i < 3 && bit(cur, i + 1) == v) ++cnt;
        if (cnt == 4) return false;
    }
    return true;
}

/* check a middle row given the row above and the row below */
static bool row_good_full(uint16_t top, uint16_t cur, uint16_t bot)
{
    for (int i = 0; i < 4; ++i) {
        unsigned v = bit(cur, i);
        unsigned cnt = 0;
        /* above row */
        if (i > 0 && bit(top, i - 1) == v) ++cnt;
        if (bit(top, i) == v) ++cnt;
        if (i < 3 && bit(top, i + 1) == v) ++cnt;
        /* same row */
        if (i > 0 && bit(cur, i - 1) == v) ++cnt;
        if (i < 3 && bit(cur, i + 1) == v) ++cnt;
        /* below row */
        if (i > 0 && bit(bot, i - 1) == v) ++cnt;
        if (bit(bot, i) == v) ++cnt;
        if (i < 3 && bit(bot, i + 1) == v) ++cnt;
        if (cnt == 4) return false;
    }
    return true;
}

/* --------------------------------------------------------------- */
/* Main sequence routine                                            */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0, but harmless */
    if (n == 1) return (seq_t)8;  /* all patterns with bit 0 = 0 */

    /* DP tables: dp[prev][curr] */
    static seq_t dp[PAT][PAT];
    static seq_t ndp[PAT][PAT];

    /* clear dp */
    for (int i = 0; i < PAT; ++i)
        for (int j = 0; j < PAT; ++j)
            dp[i][j] = 0;

    /* initialise with first two rows */
    for (uint16_t r1 = 0; r1 < PAT; ++r1) {
        if (bit(r1, 0) != 0) continue;            /* upper‑left zero */
        for (uint16_t r2 = 0; r2 < PAT; ++r2) {
            if (!row_good_no_above(r1, r2)) continue;
            dp[r1][r2] += (seq_t)1;
        }
    }

    /* iterate for rows 3 … n */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear ndp */
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                ndp[i][j] = 0;

        for (int top = 0; top < PAT; ++top) {
            for (int cur = 0; cur < PAT; ++cur) {
                seq_t ways = dp[top][cur];
                if (ways == 0) continue;
                for (int bot = 0; bot < PAT; ++bot) {
                    if (!row_good_full(top, cur, (uint16_t)bot)) continue;
                    ndp[cur][bot] += ways;
                }
            }
        }

        /* swap dp and ndp (copy to avoid pointer gymnastics) */
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final row needs to be checked with no below neighbours */
    seq_t total = 0;
    for (int top = 0; top < PAT; ++top) {
        for (int cur = 0; cur < PAT; ++cur) {
            if (dp[top][cur] == 0) continue;
            if (!row_good_no_below(top, cur)) continue;
            total += dp[top][cur];
        }
    }

    return total;
}
