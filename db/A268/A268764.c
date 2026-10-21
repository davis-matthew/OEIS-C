/*
 * A268764  Number of n X 6 binary arrays with some element plus some
 *          horizontally, vertically, diagonally or antidiagonally adjacent
 *          neighbor totalling two exactly once.
 * https://oeis.org/A268764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of n×6 binary arrays with exactly one pair of horizontally,
 * vertically, diagonally or anti‑diagonally adjacent 1s.
 */
#include <stdint.h>
#include <stdbool.h>

/* Compute OEIS sequence A268764.  External linkage as required. */
seq_t a(idx_t n)
{
    /* Edge case: the sequence is defined for n >= 1.  Return 0 if given 0. */
    if (n == 0) return (seq_t)0;

    /* Pre‑compute horizontal edges for each 6‑bit row pattern. */
    static int h[64];
    for (int pat = 0; pat < 64; ++pat)
        h[pat] = __builtin_popcount((unsigned int)(pat & (pat >> 1)));

    /* Pre‑compute cross‑row edges (vertical + two diagonals). */
    static int cross[64][64];
    for (int p = 0; p < 64; ++p)
        for (int q = 0; q < 64; ++q) {
            int v = __builtin_popcount((unsigned int)(p & q));
            int d = __builtin_popcount((unsigned int)(p & (q >> 1)));
            int a = __builtin_popcount((unsigned int)(p & (q << 1)));
            cross[p][q] = v + d + a;
        }

    /* DP arrays: dp0[pattern] – configurations with 0 adjacency pairs so far,
     * dp1[pattern] – configurations with exactly 1 adjacency pair so far.   */
    seq_t dp0[64] = {0}, dp1[64] = {0};
    seq_t new0[64], new1[64];

    /* First row: no preceding row, only horizontal edges matter. */
    for (int pat = 0; pat < 64; ++pat) {
        if (h[pat] == 0)
            dp0[pat] += (seq_t)1;
        else if (h[pat] == 1)
            dp1[pat] += (seq_t)1;
        /* h > 1 contributes nothing to sequences with ≤1 adjacency pairs. */
    }

    /* Process remaining rows.  For each new row we add horizontal edges and
     * edges connecting to the previous row.  We keep only configurations
     * with ≤1 adjacency pair. */
    for (idx_t row = 1; row < n; ++row) {
        /* Zero the new DP arrays. */
        for (int i = 0; i < 64; ++i) {
            new0[i] = 0;
            new1[i] = 0;
        }

        for (int p = 0; p < 64; ++p) {
            seq_t dp0p = dp0[p];
            seq_t dp1p = dp1[p];
            if (!dp0p && !dp1p) continue;   /* no configurations for this state */

            for (int q = 0; q < 64; ++q) {
                int tot = h[q] + cross[p][q];
                if (tot == 0) {
                    if (dp0p) new0[q] += dp0p;   /* still 0 adjacency pairs */
                    if (dp1p) new1[q] += dp1p;   /* stay at 1 pair */
                } else if (tot == 1) {
                    if (dp0p) new1[q] += dp0p;   /* 0 → 1 adjacency pair */
                    /* dp1p would become 2 adjacency pairs – discarded */
                }
            }
        }

        /* Move new arrays into dp arrays for next iteration. */
        for (int i = 0; i < 64; ++i) {
            dp0[i] = new0[i];
            dp1[i] = new1[i];
        }
    }

    /* Sum configurations that have exactly one adjacency pair after all rows. */
    seq_t result = 0;
    for (int pat = 0; pat < 64; ++pat)
        result += dp1[pat];

    return result;
}
