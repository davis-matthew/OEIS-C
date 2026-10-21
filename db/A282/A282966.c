/*
 * A282966  Number of nX5 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A282966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays with exactly two cells having
 *            more than one 1-neighbor among horizontal, vertical, and
 *            antidiagonal directions. Computed via dynamic programming over
 *            rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static int bad_count[32][32][32] = {0};

static void precompute_bad_counts(void)
{
    const int cols = 5;
    for (int A = 0; A < 32; ++A) {
        for (int B = 0; B < 32; ++B) {
            for (int C = 0; C < 32; ++C) {
                int bad = 0;
                for (int i = 0; i < cols; ++i) {
                    if (!(B & (1 << i))) continue;
                    int neigh = 0;
                    if (i > 0 && (B & (1 << (i - 1)))) neigh++;
                    if (i < cols - 1 && (B & (1 << (i + 1)))) neigh++;
                    if (A & (1 << i)) neigh++;          /* vertical up */
                    if (C & (1 << i)) neigh++;          /* vertical down */
                    if (i > 0 && (A & (1 << (i - 1)))) neigh++; /* antidiagonal NW */
                    if (i < cols - 1 && (C & (1 << (i + 1)))) neigh++; /* antidiagonal SE */
                    if (neigh > 1) bad++;
                }
                bad_count[A][B][C] = bad;
            }
        }
    }
}

seq_t a(idx_t n)
{
    static bool prepared = false;
    if (!prepared) { precompute_bad_counts(); prepared = true; }

    /* DP arrays: dp[prev][curr][bad] */
    static seq_t dp[32][32][3];
    static seq_t dp_next[32][32][3];
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            for (int k = 0; k < 3; ++k)
                dp[i][j][k] = 0;

    /* initial state: previous row is zero, current row is pattern p (row 1) */
    for (int p = 0; p < 32; ++p)
        dp[0][p][0] = 1;

    /* iterate for rows 2..n */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear next DP */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                for (int k = 0; k < 3; ++k)
                    dp_next[i][j][k] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                for (int bad = 0; bad < 3; ++bad) {
                    seq_t ways = dp[prev][curr][bad];
                    if (!ways) continue;
                    for (int next = 0; next < 32; ++next) {
                        int add = bad_count[prev][curr][next];
                        int nb = bad + add;
                        if (nb > 2) continue;
                        dp_next[curr][next][nb] += ways;
                    }
                }
            }
        }

        /* move dp_next to dp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                for (int k = 0; k < 3; ++k)
                    dp[i][j][k] = dp_next[i][j][k];
    }

    /* finalization: validate last row (current row) with next row = 0 */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            for (int bad = 0; bad < 3; ++bad) {
                seq_t ways = dp[prev][curr][bad];
                if (!ways) continue;
                int add = bad_count[prev][curr][0];
                if (bad + add == 2)
                    ans += ways;
            }
        }
    }
    return ans;
}
