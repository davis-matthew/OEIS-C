/*
 * A299841  Number of nX5 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A299841: Number of n×5 binary arrays (upper‑left element fixed to 0)
 * such that each cell has 0–8 equal king‑move neighbours, forbidding counts
 * 5 and 6.  Computed by dynamic programming over 5‑bit rows. */

#include <stdint.h>
#include <stdbool.h>

static inline bool row_ok(unsigned up, unsigned row, unsigned down,
                          bool has_up, bool has_down)
{
    for (int c = 0; c < 5; ++c) {
        unsigned val = (row >> c) & 1u;
        int cnt = 0;

        /* horizontal neighbours */
        if (c > 0     && ((row >> (c - 1)) & 1u) == val) ++cnt;
        if (c < 4     && ((row >> (c + 1)) & 1u) == val) ++cnt;

        /* upper neighbours */
        if (has_up) {
            if (((up >> c) & 1u) == val) ++cnt;
            if (c > 0 && ((up >> (c - 1)) & 1u) == val) ++cnt;
            if (c < 4 && ((up >> (c + 1)) & 1u) == val) ++cnt;
        }

        /* lower neighbours */
        if (has_down) {
            if (((down >> c) & 1u) == val) ++cnt;
            if (c > 0 && ((down >> (c - 1)) & 1u) == val) ++cnt;
            if (c < 4 && ((down >> (c + 1)) & 1u) == val) ++cnt;
        }

        /* forbidden counts */
        if (cnt == 5 || cnt == 6) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* build transition tables once */
    static bool   initialized = false;
    static bool   allow_start[32][32];
    static bool   allow_last [32][32];
    static uint32_t allow_next[32][32];

    if (!initialized) {
        /* first row has no upper neighbour, second row is its down neighbour */
        for (unsigned m0 = 0; m0 < 32; ++m0) {
            if ((m0 & 1u) == 0) {               /* upper‑left element zero */
                for (unsigned m1 = 0; m1 < 32; ++m1)
                    allow_start[m0][m1] = row_ok(0, m0, m1, false, true);
            }
        }

        /* last row has no lower neighbour */
        for (unsigned pm = 0; pm < 32; ++pm)
            for (unsigned cm = 0; cm < 32; ++cm)
                allow_last[pm][cm] = row_ok(pm, cm, 0, true, false);

        /* transitions between middle rows */
        for (unsigned pm = 0; pm < 32; ++pm)
            for (unsigned cm = 0; cm < 32; ++cm) {
                uint32_t mask = 0u;
                for (unsigned nm = 0; nm < 32; ++nm) {
                    if (row_ok(pm, cm, nm, true, true))
                        mask |= (1u << nm);
                }
                allow_next[pm][cm] = mask;
            }

        initialized = true;
    }

    /* n == 1: only the first row */
    if (n == 1) {
        seq_t total = 0;
        for (unsigned m0 = 0; m0 < 32; ++m0) {
            if ((m0 & 1u) == 0 && row_ok(0, m0, 0, false, false))
                ++total;
        }
        return total;
    }

    /* DP over rows: dp[prev][curr] = ways for last two rows */
    seq_t dp[32][32] = {{0}};
    seq_t nxt[32][32] = {{0}};

    /* initialise with first two rows */
    for (unsigned m0 = 0; m0 < 32; ++m0) {
        if ((m0 & 1u) == 0)
            for (unsigned m1 = 0; m1 < 32; ++m1)
                if (allow_start[m0][m1])
                    dp[m0][m1] = 1;
    }

    /* process remaining rows */
    for (idx_t r = 2; r < n; ++r) {
        for (unsigned i = 0; i < 32; ++i)
            for (unsigned j = 0; j < 32; ++j)
                nxt[i][j] = 0;

        for (unsigned pm = 0; pm < 32; ++pm)
            for (unsigned cm = 0; cm < 32; ++cm) {
                seq_t ways = dp[pm][cm];
                if (!ways) continue;
                uint32_t mask = allow_next[pm][cm];
                while (mask) {
                    unsigned nm = __builtin_ctz(mask);
                    mask &= mask - 1;
                    nxt[cm][nm] += ways;
                }
            }

        /* swap */
        for (unsigned i = 0; i < 32; ++i)
            for (unsigned j = 0; j < 32; ++j)
                dp[i][j] = nxt[i][j];
    }

    /* final row check */
    seq_t result = 0;
    for (unsigned pm = 0; pm < 32; ++pm)
        for (unsigned cm = 0; cm < 32; ++cm) {
            seq_t ways = dp[pm][cm];
            if (!ways) continue;
            if (allow_last[pm][cm])
                result += ways;
        }

    return result;
}
