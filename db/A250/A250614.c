/*
 * A250614  Number of (n+1)X(2+1) 0..2 arrays with nondecreasing maximum of
 *          every two consecutive values in every row and column.
 * https://oeis.org/A250614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 18, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A250614 by dynamic programming over valid rows.
   The algorithm enumerates the 22 row patterns of length 3 over {0,1,2}
   that satisfy the horizontal constraint max(a[0],a[1]) ≤ max(a[1],a[2]).
   For each step, it keeps the last two rows and counts all arrays
   whose vertical maxima between successive rows form a nondecreasing
   sequence.  Complexity: O(n * 22^3) which is well below the limit
   for n up to 10 000. */
#include <stddef.h>
#include <stdbool.h>

static int rows[22][3];
static bool allowed[22][22][22];
static bool initialized = false;

static void init(void)
{
    if (initialized) {
        return;
    }
    int idx = 0;
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            for (int z = 0; z < 3; ++z) {
                int m1 = (x > y) ? x : y;
                int m2 = (y > z) ? y : z;
                if (m1 <= m2) {
                    rows[idx][0] = x;
                    rows[idx][1] = y;
                    rows[idx][2] = z;
                    ++idx;
                }
            }
        }
    }
    /* idx should be 22 */
    for (int p = 0; p < 22; ++p) {
        for (int q = 0; q < 22; ++q) {
            for (int r = 0; r < 22; ++r) {
                bool ok = true;
                for (int c = 0; c < 3; ++c) {
                    int mp = (rows[p][c] > rows[q][c]) ? rows[p][c] : rows[q][c];
                    int mq = (rows[q][c] > rows[r][c]) ? rows[q][c] : rows[r][c];
                    if (mp > mq) {
                        ok = false;
                        break;
                    }
                }
                allowed[p][q][r] = ok;
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    init();

    static seq_t dp[22][22];
    static seq_t ndp[22][22];

    /* initialise all pairs (p,q) when only two rows are present */
    for (int p = 0; p < 22; ++p) {
        for (int q = 0; q < 22; ++q) {
            dp[p][q] = (seq_t)1;
        }
    }

    if (n > 1) {
        for (idx_t step = 2; step <= n; ++step) {
            /* reset ndp */
            for (int i = 0; i < 22; ++i) {
                for (int j = 0; j < 22; ++j) {
                    ndp[i][j] = (seq_t)0;
                }
            }

            for (int p = 0; p < 22; ++p) {
                for (int q = 0; q < 22; ++q) {
                    seq_t val = dp[p][q];
                    if (val == (seq_t)0) {
                        continue;
                    }
                    for (int r = 0; r < 22; ++r) {
                        if (allowed[p][q][r]) {
                            ndp[q][r] = ndp[q][r] + val;
                        }
                    }
                }
            }
            /* swap dp and ndp for next iteration */
            for (int i = 0; i < 22; ++i) {
                for (int j = 0; j < 22; ++j) {
                    dp[i][j] = ndp[i][j];
                }
            }
        }
    }

    /* sum all final states */
    seq_t total = (seq_t)0;
    for (int p = 0; p < 22; ++p) {
        for (int q = 0; q < 22; ++q) {
            total = total + dp[p][q];
        }
    }

    return total;
}
