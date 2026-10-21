/*
 * A297548  Number of 5Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2 or 3 neighboring 1s.
 * https://oeis.org/A297548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP over 5×n binary arrays where each 1 has 2 or 3
 * horizontally, diagonally or antidiagonally adjacent 1s. */

/* The code implements the exact definition from OEIS A297548 by
 * scanning columns left-to-right and keeping the last two columns
 * as state.  For each column the adjacency requirement is checked
 * with its left, right, and diagonal neighbours.  The DP table
 * dimension is 32×32 (5-bit masks for two columns).  The transition
 * matrix is precomputed once.  The function works for any seq_t
 * supplied by the build and is independent of n (calls may come
 * in any order). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety, offset is 1 */

    /* --------- Pre‑compute transition bits (32×32) --------- */
    static bool done = false;
    static unsigned int trans[32][32];   /* bitmask over next column 0..31 */
    if (!done) {
        for (unsigned int a = 0; a < 32; ++a) {
            for (unsigned int b = 0; b < 32; ++b) {
                unsigned int mask = 0;
                for (unsigned int c = 0; c < 32; ++c) {
                    bool ok = true;
                    for (int r = 0; r < 5 && ok; ++r) {
                        if ((b >> r) & 1U) {        /* 1 in column b, row r */
                            int cnt = 0;
                            /* horizontal neighbours */
                            if ((a >> r) & 1U) cnt++;   /* left */
                            if ((c >> r) & 1U) cnt++;   /* right */
                            /* diagonal neighbours (left) */
                            if (r > 0 && ((a >> (r - 1)) & 1U)) cnt++;
                            if (r < 4 && ((a >> (r + 1)) & 1U)) cnt++;
                            /* diagonal neighbours (right) */
                            if (r > 0 && ((c >> (r - 1)) & 1U)) cnt++;
                            if (r < 4 && ((c >> (r + 1)) & 1U)) cnt++;
                            /* note: vertical neighbours are NOT
                             * counted – they are irrelevant for this
                             * definition. */
                            if (cnt < 2 || cnt > 3) ok = false;
                        }
                    }
                    if (ok) mask |= (1U << c);
                }
                trans[a][b] = mask;
            }
        }
        done = true;
    }

    /* --------- DP tables 32×32 (two buffers) --------- */
    static seq_t dp0[32][32];
    static seq_t dp1[32][32];
    seq_t (*cur)[32] = dp0;
    seq_t (*nxt)[32] = dp1;

    /* initialise current table */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            cur[i][j] = 0;
    cur[0][0] = 1;                     /* two sentinel zero columns */

    /* process each column of the matrix */
    for (idx_t col = 0; col < n; ++col) {
        /* clear next table */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                nxt[i][j] = 0;

        /* transition */
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                seq_t ways = cur[a][b];
                if (ways == 0) continue;
                unsigned int bits = trans[a][b];
                while (bits) {
                    int c = __builtin_ctz(bits);
                    bits &= bits - 1;
                    nxt[b][c] += ways;
                }
            }
        }

        /* swap buffers */
        seq_t (*tmp)[32] = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* final sentinel column (zeros) – validate last real column */
    seq_t result = 0;
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            seq_t ways = cur[a][b];
            if (ways == 0) continue;
            if (trans[a][b] & 1U)            /* next column = 0 allowed ? */
                result += ways;
        }
    }
    return result;
}
