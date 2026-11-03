/*
 * A250442  Number of (n+1)X(7+1) 0..2 arrays with nondecreasing sum of every
 *          two consecutive values in every row and column.
 * https://oeis.org/A250442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 14, u128 n <= 79,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 169 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A250442
 *
 *  a(n) = number of (n+1)×8 arrays with entries 0,1,2 such that
 *  the sum of every pair of consecutive entries in each row and each
 *  column is nondecreasing.  The array satisfies
 *  a[i][j] ≤ a[i][j+2]   and   a[i][j] ≤ a[i+2][j]  for all applicable indices,
 *  which decomposes the array into four independent R×4 grids.
 *
 *  For R rows of the same parity, let f(R) be the number of
 *  0–2 R×4 grids satisfying the above inequalities.
 *  Then
 *      a(n) = f(⌊n/2⌋+1)^2  ·  f(⌊(n+1)/2⌋)^2 .
 *
 *  f(R) is computed by a simple DP over the 15 nondecreasing 4‑tuples
 *  of 0,1,2.  No precomputed tables are used; all comparisons are
 *  performed at run time.
 *
 *  The implementation uses only integer types and operators allowed
 *  by the problem statement.  It builds the comparison matrix once
 *  and reuses it for every call.  The code works for arbitrary‑precision
 *  seq_t as well as for fixed‑size uint64_t, etc.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t f_compute(idx_t R)
{
    /* All weakly increasing 4‑tuples over {0,1,2}. */
    static const int patterns[15][4] = {
        {0,0,0,0}, {0,0,0,1}, {0,0,0,2},
        {0,0,1,1}, {0,0,1,2}, {0,0,2,2},
        {0,1,1,1}, {0,1,1,2}, {0,1,2,2},
        {0,2,2,2}, {1,1,1,1}, {1,1,1,2},
        {1,1,2,2}, {1,2,2,2}, {2,2,2,2}
    };

    /* Precompute comparability once per program run. */
    static bool comp[15][15];
    static bool init = false;
    if (!init) {
        for (int q = 0; q < 15; ++q)
            for (int p = 0; p < 15; ++p) {
                bool le = true;
                for (int k = 0; k < 4; ++k)
                    if (patterns[q][k] > patterns[p][k]) { le = false; break; }
                comp[q][p] = le;
            }
        init = true;
    }

    seq_t dp[15];
    seq_t next[15];
    for (int p = 0; p < 15; ++p) dp[p] = 1;        /* first row can be any pattern */

    for (idx_t r = 1; r < R; ++r) {                 /* add remaining R-1 rows */
        for (int p = 0; p < 15; ++p) next[p] = 0;
        for (int q = 0; q < 15; ++q) {
            seq_t val = dp[q];
            if (!val) continue;
            for (int p = 0; p < 15; ++p)
                if (comp[q][p]) next[p] += val;
        }
        for (int p = 0; p < 15; ++p) dp[p] = next[p];
    }

    seq_t sum = 0;
    for (int p = 0; p < 15; ++p) sum += dp[p];
    return sum;
}

/* ------------------------------------------------------------------ */
/*  Sequence definition                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Number of rows with even index 0..n : floor(n/2)+1  */
    idx_t R_even = (n / 2) + 1;
    /* Number of rows with odd  index 0..n : floor((n+1)/2) */
    idx_t R_odd  = (n + 1) / 2;

    seq_t fe = f_compute(R_even);
    seq_t fo = f_compute(R_odd);

    seq_t res = fe * fe;
    seq_t tmp = fo * fo;
    res = res * tmp;
    return res;
}
