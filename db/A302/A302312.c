/*
 * A302312  Number of 4 X n 0..1 arrays with every element equal to 0, 1, 3 or
 *          4 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A302312: Number of 4×n 0/1 arrays with
 * every element equal to 0, 1, 3 or 4 among its
 * horizontally, diagonally or antidiagonally adjacent elements.
 * The upper-left element must be 0.  The index starts at 1. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Tables of admissible triples / pairs (built once). */
    static bool valid_start[16][16];
    static bool valid_mid[16][16][16];
    static bool valid_end[16][16];
    static bool tables_built = false;

    if (!tables_built) {
        /* pairs for the first or last column (no left or right neighbour) */
        for (int pre = 0; pre < 16; ++pre) {
            for (int cur = 0; cur < 16; ++cur) {
                /* start pair : column pre has no left neighbour, only right (cur) */
                bool ok = true;
                for (int r = 0; r < 4; ++r) {
                    uint8_t prevbit = (pre >> r) & 1;
                    uint8_t cnt = 0;

                    /* neighbour on the same row to the right */
                    if (((cur >> r) & 1) == prevbit) ++cnt;
                    /* up‑right diagonal */
                    if (r > 0 && ((cur >> (r - 1)) & 1) == prevbit) ++cnt;
                    /* down‑right diagonal */
                    if (r < 3 && ((cur >> (r + 1)) & 1) == prevbit) ++cnt;

                    if (!(cnt == 0 || cnt == 1 || cnt == 3)) { ok = false; break; }
                }
                valid_start[pre][cur] = ok;

                /* end pair : column cur has no right neighbour, only left (pre) */
                ok = true;
                for (int r = 0; r < 4; ++r) {
                    uint8_t curbit = (cur >> r) & 1;
                    uint8_t cnt = 0;

                    /* neighbour on the same row to the left */
                    if (((pre >> r) & 1) == curbit) ++cnt;
                    /* up‑left diagonal */
                    if (r > 0 && ((pre >> (r - 1)) & 1) == curbit) ++cnt;
                    /* down‑left diagonal */
                    if (r < 3 && ((pre >> (r + 1)) & 1) == curbit) ++cnt;

                    if (!(cnt == 0 || cnt == 1 || cnt == 3)) { ok = false; break; }
                }
                valid_end[pre][cur] = ok;
            }
        }

        /* triple table for valid middle column (both neighbours exist) */
        for (int preprev = 0; preprev < 16; ++preprev) {
            for (int pre = 0; pre < 16; ++pre) {
                for (int cur = 0; cur < 16; ++cur) {
                    bool ok = true;
                    for (int r = 0; r < 4; ++r) {
                        uint8_t prevbit = (pre >> r) & 1;
                        uint8_t cnt = 0;

                        /* left side: same row */
                        if (((preprev >> r) & 1) == prevbit) ++cnt;
                        /* left side: up‑left diagonal */
                        if (r > 0 && ((preprev >> (r - 1)) & 1) == prevbit) ++cnt;
                        /* left side: down‑left diagonal */
                        if (r < 3 && ((preprev >> (r + 1)) & 1) == prevbit) ++cnt;

                        /* right side: same row */
                        if (((cur >> r) & 1) == prevbit) ++cnt;
                        /* right side: up‑right diagonal */
                        if (r > 0 && ((cur >> (r - 1)) & 1) == prevbit) ++cnt;
                        /* right side: down‑right diagonal */
                        if (r < 3 && ((cur >> (r + 1)) & 1) == prevbit) ++cnt;

                        if (!(cnt == 0 || cnt == 1 || cnt == 3 || cnt == 4)) {
                            ok = false; break;
                        }
                    }
                    valid_mid[preprev][pre][cur] = ok;
                }
            }
        }
        tables_built = true;
    }

    /* Offset: a(1) corresponds to a single column of width 1. */
    if (n == 1) return (seq_t)8;   /* 3 free rows, upper-left forced to 0 */

    /* dp[prev][cur]: number of ways to build columns up to cur,
     * with columns (prev, cur) currently stored and prev already validated
     * (its right neighbour is cur). */
    seq_t dp[16][16] = {0};

    /* Initialise with first two columns.  Column 1 has no left neighbour,
     * only right neighbour (column 2). */
    for (int col1 = 0; col1 < 16; ++col1) {
        if ((col1 & 1) != 0) continue;   /* upper‑left element must be 0 */
        for (int col2 = 0; col2 < 16; ++col2) {
            if (valid_start[col1][col2]) {
                dp[col1][col2] += (seq_t)1;
            }
        }
    }

    /* Iterate columns 3 … n.  At each step the new column 'cur' validates
     * the middle column 'base' (previously stored as 'prev'). */
    for (idx_t col = 3; col <= n; ++col) {
        seq_t ndp[16][16] = {0};
        for (int prevprev = 0; prevprev < 16; ++prevprev) {
            for (int prev = 0; prev < 16; ++prev) {
                seq_t ways = dp[prevprev][prev];
                if (ways == 0) continue;
                for (int cur = 0; cur < 16; ++cur) {
                    if (valid_mid[prevprev][prev][cur]) {
                        ndp[prev][cur] += ways;
                    }
                }
            }
        }
        /* copy ndp back into dp for next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Final column has no right neighbour – validate it. */
    seq_t result = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int cur = 0; cur < 16; ++cur) {
            if (valid_end[prev][cur]) {
                result += dp[prev][cur];
            }
        }
    }
    return result;
}
