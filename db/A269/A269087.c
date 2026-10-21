/*
 * A269087  Number of nX6 binary arrays with some element plus some
 *          horizontally, vertically or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A269087
   Number of n×6 binary arrays with at most one pair of adjacent 1s
   (adjacent horizontally, vertically or anti‑diagonally). This
   implementation uses dynamic programming over rows. For each 6‑bit
   row pattern we pre‑compute its horizontal adjacency count and the
   vertical/anti‑diagonal adjacency contribution to the next row.
   States dp0[mask] and dp1[mask] store the number of arrays reaching
   the current row with 0 or 1 adjacency pair, respectively.  The
   algorithm runs in O(n·64²) time, which is well below the
   2‑second constraint for n ≤ 10000. */

#include <stdint.h>

/* External types are supplied by the build environment. */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    if (n == 0) /* sequence starts at n = 1 */
        return (seq_t)0;

    /* Precomputation: horizontal adjacency counts for all 6‑bit masks
       and valid transitions between consecutive rows. */
    static int initialized = 0;
    static int horiz[64];
    static int8_t valid_next[64][64];   /* -1 : invalid, 0 or 1 : new adjacency pairs */
    if (!initialized) {
        for (int m = 0; m < 64; ++m) {
            int cnt = 0;
            for (int j = 0; j < 5; ++j)
                if (((m >> j) & 1) && ((m >> (j + 1)) & 1))
                    ++cnt;
            horiz[m] = cnt;
        }

        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                int v = 0;   /* vertical adjacency (same column) */
                for (int j = 0; j < 6; ++j)
                    if (((a >> j) & 1) && ((b >> j) & 1))
                        ++v;
                int u = 0;   /* anti‑diagonal adjacency (row r+1, col j-1) */
                for (int j = 0; j < 5; ++j)
                    if (((a >> (j + 1)) & 1) && ((b >> j) & 1))
                        ++u;
                int tot = v + u;
                int inrow_next = horiz[b];
                if (inrow_next > 1 || tot > 1 || (inrow_next + tot) > 1)
                    valid_next[a][b] = -1;      /* invalid transition */
                else
                    valid_next[a][b] = (int8_t)(inrow_next + tot);
            }
        }
        initialized = 1;
    }

    /* DP arrays: counts for arrays ending with a particular mask,
       having 0 or 1 adjacency pair used so far. */
    seq_t dp0[64], dp1[64];
    seq_t ndp0[64], ndp1[64];

    /* Clear DP arrays */
    for (int i = 0; i < 64; ++i) {
        dp0[i] = (seq_t)0;
        dp1[i] = (seq_t)0;
    }

    /* Initialise with the first row */
    for (int m = 0; m < 64; ++m) {
        if (horiz[m] == 0)
            dp0[m] = (seq_t)1;
        else if (horiz[m] == 1)
            dp1[m] = (seq_t)1;
        /* masks with horiz > 1 are discarded automatically */
    }

    /* Process remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        /* Clear next‑row DP arrays */
        for (int i = 0; i < 64; ++i) {
            ndp0[i] = (seq_t)0;
            ndp1[i] = (seq_t)0;
        }

        for (int p = 0; p < 64; ++p) {
            seq_t ways0 = dp0[p];
            seq_t ways1 = dp1[p];
            if (ways0 == (seq_t)0 && ways1 == (seq_t)0)
                continue;

            for (int c = 0; c < 64; ++c) {
                int8_t add = valid_next[p][c];
                if (add < 0)
                    continue;   /* invalid transition */

                if (add == 0) {
                    ndp0[c] += ways0;
                    ndp1[c] += ways1;
                } else {   /* add == 1 */
                    ndp1[c] += ways0;   /* only from states with 0 pairs */
                    /* states with 1 pair cannot transition with an extra pair */
                }
            }
        }

        /* copy ndp to dp for next iteration */
        for (int i = 0; i < 64; ++i) {
            dp0[i] = ndp0[i];
            dp1[i] = ndp1[i];
        }
    }

    /* Sum all valid configurations (0 or 1 pair used) */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 64; ++i) {
        result += dp0[i];
        result += dp1[i];
    }
    return result;
}
