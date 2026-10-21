/*
 * A251314  Number of (n+1)X(5+1) 0..1 arrays with no 2X2 subblock having the
 *          maximum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over rows using the adjacency
 * derived from the 2×2 subblock condition:
 * a forbidden subblock occurs when (a | d) && (b == c),
 * where a = top‑left, b = top‑right, c = bottom‑left, d = bottom‑right
 * for columns j and j+1.  The function counts (n+1)×6 binary matrices
 * with no such forbidden subblock.  No memoisation beyond a single
 * DP pass is used, so the algorithm is O(64²·n) and fast for n up to 10⁴. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* adjacency table: adj[r][s] is 1 iff a row pattern s can follow row r */
static uint8_t adj[64][64];
static bool adj_inited = false;

static void init_adj(void)
{
    if (adj_inited) return;
    for (unsigned r = 0; r < 64; ++r)
        for (unsigned s = 0; s < 64; ++s) {
            bool ok = true;
            for (unsigned j = 0; j < 5 && ok; ++j) {
                unsigned a = (r >> j)     & 1u;
                unsigned b = (r >> (j + 1)) & 1u;
                unsigned c = (s >> j)     & 1u;
                unsigned d = (s >> (j + 1)) & 1u;
                if ((a | d) && (b == c))
                    ok = false;
            }
            adj[r][s] = ok ? 1 : 0;
        }
    adj_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, undefined for n==0 */
        return 0;

    init_adj();

    seq_t dp_cur[64];
    seq_t dp_next[64];

    /* initial state: one row of any pattern */
    for (unsigned i = 0; i < 64; ++i)
        dp_cur[i] = 1;

    /* iterate from 1 row to n+1 rows */
    for (idx_t step = 1; step <= n; ++step) {
        /* clear next state */
        for (unsigned s = 0; s < 64; ++s)
            dp_next[s] = 0;

        /* transition */
        for (unsigned r = 0; r < 64; ++r) {
            seq_t val = dp_cur[r];
            if (val == 0) continue;
            for (unsigned s = 0; s < 64; ++s)
                if (adj[r][s])
                    dp_next[s] += val;
        }

        /* copy next to current for next iteration */
        for (unsigned s = 0; s < 64; ++s)
            dp_cur[s] = dp_next[s];
    }

    /* sum of all ending rows gives the number of arrays of size n+1 */
    seq_t result = 0;
    for (unsigned s = 0; s < 64; ++s)
        result += dp_cur[s];

    return result;
}
