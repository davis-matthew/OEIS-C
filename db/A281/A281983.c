/*
 * A281983  Number of nX3 0..1 arrays with no element unequal to more than
 *          four of its king-move neighbors, with the exception of exactly one
 *          element, and with new values introduced in order 0 sequentially
 *          upwards.
 * https://oeis.org/A281983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of n×3 0..1 arrays with no element unequal to more than
 * four of its king‑move neighbors, with the exception of exactly one element,
 * and with new values introduced in order 0 sequentially upwards.
 *
 * The array is counted only if the first element (row 0, column 0) is 0
 * – this enforces the "new values introduced in order 0 sequentially upwards"
 * rule.  The dynamic‑programming algorithm enumerates all rows while
 * accumulating the number of violating cells; the total number of
 * violations must be exactly one.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* The value 8 indicates a missing row (sentinel). */
static const idx_t SENTINEL = 8U;

/* Pre‑compute the number of violating cells in the middle row of three
 * consecutive rows.  prev, curr, next range over 0..7 (three‑bit patterns)
 * with SENTINEL representing the absence of a row. */
static void precompute_violation_tables(int VIOL[9][8][8], int LASTVIOL[9][8])
{
    for (int prev = 0; prev < 9; ++prev) {
        bool prev_ok = (prev != SENTINEL);
        for (int curr = 0; curr < 8; ++curr) {
            for (int next = 0; next < 8; ++next) {
                int v = 0;
                for (int j = 0; j < 3; ++j) {
                    int curr_bit = (curr >> j) & 1;
                    int diff = 0;
                    /* up‑left */
                    if (prev_ok && j > 0) {
                        int nb = (prev >> (j - 1)) & 1;
                        if (nb != curr_bit) diff++;
                    }
                    /* up */
                    if (prev_ok) {
                        int nb = (prev >> j) & 1;
                        if (nb != curr_bit) diff++;
                    }
                    /* up‑right */
                    if (prev_ok && j < 2) {
                        int nb = (prev >> (j + 1)) & 1;
                        if (nb != curr_bit) diff++;
                    }
                    /* left */
                    if (j > 0) {
                        int nb = (curr >> (j - 1)) & 1;
                        if (nb != curr_bit) diff++;
                    }
                    /* right */
                    if (j < 2) {
                        int nb = (curr >> (j + 1)) & 1;
                        if (nb != curr_bit) diff++;
                    }
                    /* down‑left */
                    if (true && j > 0) {          /* next always exists in this context */
                        int nb = (next >> (j - 1)) & 1;
                        if (nb != curr_bit) diff++;
                    }
                    /* down */
                    if (true) {
                        int nb = (next >> j) & 1;
                        if (nb != curr_bit) diff++;
                    }
                    /* down‑right */
                    if (true && j < 2) {
                        int nb = (next >> (j + 1)) & 1;
                        if (nb != curr_bit) diff++;
                    }
                    if (diff > 4) v++;
                }
                VIOL[prev][curr][next] = v;
            }

            /* Now treat the case where there is no next row */
            int v = 0;
            for (int j = 0; j < 3; ++j) {
                int curr_bit = (curr >> j) & 1;
                int diff = 0;
                /* up‑left */
                if (prev_ok && j > 0) {
                    int nb = (prev >> (j - 1)) & 1;
                    if (nb != curr_bit) diff++;
                }
                /* up */
                if (prev_ok) {
                    int nb = (prev >> j) & 1;
                    if (nb != curr_bit) diff++;
                }
                /* up‑right */
                if (prev_ok && j < 2) {
                    int nb = (prev >> (j + 1)) & 1;
                    if (nb != curr_bit) diff++;
                }
                /* left */
                if (j > 0) {
                    int nb = (curr >> (j - 1)) & 1;
                    if (nb != curr_bit) diff++;
                }
                /* right */
                if (j < 2) {
                    int nb = (curr >> (j + 1)) & 1;
                    if (nb != curr_bit) diff++;
                }
                /* no down neighbors */
                if (diff > 4) v++;
            }
            LASTVIOL[prev][curr] = v;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1 */

    /* Lazy static initialisation of the tables */
    static bool init_done = false;
    static int viol[9][8][8];
    static int viol_last[9][8];
    if (!init_done) {
        precompute_violation_tables(viol, viol_last);
        init_done = true;
    }

    /* DP: dp[prev_row][curr_row][used_violation] */
    seq_t dp[9][8][2]      = { {0} };
    seq_t dp_next[9][8][2] = { {0} };

    /* Initial state: first row chosen, no violation yet.
     * Only allow patterns whose first element (column 0) is 0. */
    for (int p0 = 0; p0 < 8; ++p0) {
        if ((p0 & 1) == 0) /* first column bit 0 */ {
            dp[SENTINEL][p0][0] = 1;
        }
    }

    /* Process rows 1 .. n-1 */
    for (idx_t row = 1; row < n; ++row) {
        /* clear next DP table */
        for (int p = 0; p < 9; ++p)
            for (int c = 0; c < 8; ++c)
                for (int v = 0; v < 2; ++v)
                    dp_next[p][c][v] = 0;

        for (int prev = 0; prev < 9; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                for (int used = 0; used < 2; ++used) {
                    seq_t ways = dp[prev][curr][used];
                    if (!ways) continue;
                    for (int nxt = 0; nxt < 8; ++nxt) {
                        int add = viol[prev][curr][nxt];
                        if (used + add > 1) continue;
                        dp_next[curr][nxt][used + add] += ways;
                    }
                }
            }
        }

        /* swap dp and dp_next */
        for (int p = 0; p < 9; ++p)
            for (int c = 0; c < 8; ++c)
                for (int v = 0; v < 2; ++v)
                    dp[p][c][v] = dp_next[p][c][v];
    }

    /* Final row evaluation */
    seq_t result = 0;
    for (int prev = 0; prev < 9; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            for (int used = 0; used < 2; ++used) {
                seq_t ways = dp[prev][curr][used];
                if (!ways) continue;
                int add = viol_last[prev][curr];
                int total = used + add;
                if (total == 1)
                    result += ways;
            }
        }
    }
    return result;
}
