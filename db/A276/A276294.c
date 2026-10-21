/*
 * A276294  Number of n X 3 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-1,1) or (0,-1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A276294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2} with the * constraints:
 *            * 1. for each element at (i,j), it must differ from * (i-2,j-1),
 *            (i-1,j+1), and (i,j-1); * 2. new values are introduced in order
 *            0,1,2 (first new value * must be 0, the next new value 1, etc.).
 *            * * The algorithm uses dynamic program...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   next_unused_colour
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_unused_colour.h"

#define PATCNT 12   /* number of admissible row patterns */

/* ------------------------------------------------------------------ */
/* Helper: generate the 12 admissible patterns (a,b,c) with a!=b!=c */
/* ------------------------------------------------------------------ */
static void generate_patterns(int pat[PATCNT][3]) {
    int idx = 0;
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                if (a != b && b != c) {
                    pat[idx][0] = a;
                    pat[idx][1] = b;
                    pat[idx][2] = c;
                    ++idx;
                }
}

/* ------------------------------------------------------------------ */
/* Smallest colour not yet used in mask (0‑based)                     */

/* Transition table: transition[p][pre] == post mask, or -1 if invalid */
/* ------------------------------------------------------------------ */
static int transition[ PATCNT ][ 8 ];

/* Compatibility tables for spatial constraints */
/* ------------------------------------------------ */
static bool compat1[ PATCNT ][ PATCNT ];   /* offset (-1, +1)  */
static bool compat2[ PATCNT ][ PATCNT ];   /* offset (-2, -1)  */

/* Pre‑compute all tables once */
static void init_tables(void) {
    static bool inited = false;
    if (inited) return;
    inited = true;

    int pattern[ PATCNT ][ 3 ];
    generate_patterns(pattern);

    /* Transition table */
    for (int p = 0; p < PATCNT; ++p) {
        for (int pre = 0; pre < 8; ++pre) {
            int m = pre;
            bool ok = true;
            for (int j = 0; j < 3 && ok; ++j) {
                int val = pattern[p][j];
                if (m & (1 << val)) { /* already used */
                    /* nothing to check */
                } else {              /* new colour */
                    int nxt = next_unused_colour(m);
                    if (val != nxt) ok = false;
                    else m |= (1 << val);
                }
            }
            transition[p][pre] = ok ? m : -1;
        }
    }

    /* Compatibility for offset (-1,+1) */
    for (int a = 0; a < PATCNT; ++a)
        for (int b = 0; b < PATCNT; ++b) {
            bool ok = true;
            for (int j = 0; j < 2 && ok; ++j)
                if (pattern[b][j] == pattern[a][j + 1]) ok = false;
            compat1[a][b] = ok;
        }

    /* Compatibility for offset (-2,-1) */
    for (int a = 0; a < PATCNT; ++a)
        for (int b = 0; b < PATCNT; ++b) {
            bool ok = true;
            for (int j = 1; j < 3 && ok; ++j)
                if (pattern[b][j] == pattern[a][j - 1]) ok = false;
            compat2[a][b] = ok;
        }
}

/* ------------------------------------------------------------------ */
/* Sequence function                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    init_tables();

    /* DP arrays: counts of partial arrays ending with given patterns
     * and used colour mask.
     */
    seq_t dp_first[ PATCNT ][ 8 ]         = { {0} };
    seq_t dp_last2[ PATCNT ][ PATCNT ][ 8 ] = { { {0} } };
    seq_t dp_curr  [ PATCNT ][ PATCNT ][ 8 ] = { { {0} } };
    seq_t dp_next  [ PATCNT ][ PATCNT ][ 8 ] = { { {0} } };

    /* First row : only start from mask 0 */
    for (int p = 0; p < PATCNT; ++p) {
        int post = transition[p][0];
        if (post != -1) dp_first[p][post] = 1;
    }

    if (n == 1) {
        seq_t res = 0;
        for (int p = 0; p < PATCNT; ++p)
            for (int m = 0; m < 8; ++m)
                res += dp_first[p][m];
        return res;
    }

    /* Second row */
    for (int a = 0; a < PATCNT; ++a) {
        for (int m1 = 0; m1 < 8; ++m1) {
            seq_t cnt = dp_first[a][m1];
            if (!cnt) continue;
            for (int b = 0; b < PATCNT; ++b) {
                int m2 = transition[b][m1];
                if (m2 == -1) continue;
                if (!compat1[a][b]) continue;  /* (i‑1, j+1) check */
                dp_last2[a][b][m2] += cnt;
            }
        }
    }

    if (n == 2) {
        seq_t res = 0;
        for (int a = 0; a < PATCNT; ++a)
            for (int b = 0; b < PATCNT; ++b)
                for (int m = 0; m < 8; ++m)
                    res += dp_last2[a][b][m];
        return res;
    }

    /* Initialize dp_curr with state after two rows */
    for (int a = 0; a < PATCNT; ++a)
        for (int b = 0; b < PATCNT; ++b)
            for (int m = 0; m < 8; ++m)
                dp_curr[a][b][m] = dp_last2[a][b][m];

    /* Process rows 3 … n */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero dp_next */
        for (int i = 0; i < PATCNT; ++i)
            for (int j = 0; j < PATCNT; ++j)
                for (int k = 0; k < 8; ++k)
                    dp_next[i][j][k] = 0;

        for (int a = 0; a < PATCNT; ++a)
            for (int b = 0; b < PATCNT; ++b)
                for (int m = 0; m < 8; ++m) {
                    seq_t cnt = dp_curr[a][b][m];
                    if (!cnt) continue;
                    for (int c = 0; c < PATCNT; ++c) {
                        int m2 = transition[c][m];
                        if (m2 == -1) continue;
                        if (!compat1[b][c] || !compat2[a][c]) continue;
                        dp_next[b][c][m2] += cnt;
                    }
                }

        /* copy next -> curr */
        for (int i = 0; i < PATCNT; ++i)
            for (int j = 0; j < PATCNT; ++j)
                for (int k = 0; k < 8; ++k)
                    dp_curr[i][j][k] = dp_next[i][j][k];
    }

    /* Sum all complete rows */
    seq_t res = 0;
    for (int a = 0; a < PATCNT; ++a)
        for (int b = 0; b < PATCNT; ++b)
            for (int m = 0; m < 8; ++m)
                res += dp_curr[a][b][m];
    return res;
}
