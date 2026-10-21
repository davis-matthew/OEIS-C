/*
 * A254849  Number of (n+2)X(3+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum one and no antidiagonal sum two.
 * https://oeis.org/A254849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×5 binary arrays with no 3×3 subblock
 *            main‑diagonal sum 1 and no antidiagonal sum 2. Computed via
 *            dynamic programming over rows
 * Range:     u8 none, u16 n <= 5, u32 n <= 17, u64 n <= 42, u128 n <= 91,
 *            big any n
 * Helpers:   valid_triplet
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/valid_triplet.h"

seq_t a(idx_t n)
{
    idx_t rows = n + (idx_t)2;         /* total number of rows */
    idx_t steps = rows - (idx_t)2;     /* number of transitions needed */

    /* Precompute allowed triples once per program run. */
    static char ok[32][32][32];
    static bool init = false;
    if (!init) {
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                for (int c = 0; c < 32; ++c)
                    ok[a][b][c] = valid_triplet(a, b, c);
        init = true;
    }

    /* DP arrays: dp[a][b] = number of ways ending with rows a, b. */
    seq_t dp[32][32];
    seq_t ndp[32][32];

    /* Start with all pairs of first two rows allowed. */
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            dp[a][b] = 1;

    /* Iterate over remaining rows. */
    for (idx_t step = 0; step < steps; ++step) {
        /* Zero ndp. */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                ndp[a][b] = 0;

        /* Transition. */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b) {
                seq_t val = dp[a][b];
                if (val == 0) continue;
                for (int c = 0; c < 32; ++c)
                    if (ok[a][b][c])
                        ndp[b][c] += val;
            }

        /* Copy ndp back to dp for next step. */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                dp[a][b] = ndp[a][b];
    }

    /* Sum all ways after final row. */
    seq_t result = 0;
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            result += dp[a][b];

    return result;
}
