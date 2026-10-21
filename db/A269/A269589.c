/*
 * A269589  Number of triples 0 <= i, j, k < n such that bitwise AND of i, j,
 *          k is 0.
 * https://oeis.org/A269589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples 0 <= i, j, k < n such that (i & j & k)
 *            == 0
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Recursive digit DP helper */
static seq_t dfs(int pos, int t1, int t2, int t3,
                 idx_t n_minus1,
                 bool visited[65][2][2][2],
                 seq_t memo[65][2][2][2])
{
    if (pos < 0)
        return (seq_t)1;

    if (visited[pos][t1][t2][t3])
        return memo[pos][t1][t2][t3];

    int limit1 = t1 ? (int)((n_minus1 >> pos) & (idx_t)1) : 1;
    int limit2 = t2 ? (int)((n_minus1 >> pos) & (idx_t)1) : 1;
    int limit3 = t3 ? (int)((n_minus1 >> pos) & (idx_t)1) : 1;

    seq_t res = 0;

    for (int b1 = 0; b1 <= limit1; ++b1)
        for (int b2 = 0; b2 <= limit2; ++b2)
            for (int b3 = 0; b3 <= limit3; ++b3) {
                /* forbid (1,1,1) */
                if (b1 == 1 && b2 == 1 && b3 == 1)
                    continue;

                int nt1 = t1 && (b1 == limit1);
                int nt2 = t2 && (b2 == limit2);
                int nt3 = t3 && (b3 == limit3);

                res += dfs(pos - 1, nt1, nt2, nt3,
                           n_minus1, visited, memo);
            }

    visited[pos][t1][t2][t3] = true;
    memo[pos][t1][t2][t3] = res;
    return res;
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    /* initialize memoisation tables */
    static bool visited[65][2][2][2];
    static seq_t memo[65][2][2][2];
    for (int i = 0; i < 65; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
                for (int l = 0; l < 2; ++l)
                    visited[i][j][k][l] = false;

    /* handle lower bound n >= 1 (offset of OEIS) */
    idx_t nm1 = n - 1;          /* largest number in the range [0, n-1] */

    /* find highest set bit position of nm1 (at least 0) */
    int maxpos = 0;
    for (idx_t t = nm1; t > 1; t >>= 1)
        ++maxpos;

    /* run the DP */
    return dfs(maxpos, 1, 1, 1, nm1, visited, memo);
}
