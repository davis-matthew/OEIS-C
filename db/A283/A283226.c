/*
 * A283226  Number of n X 2 0..1 arrays with no 1 equal to more than two of
 *          its horizontal and vertical neighbors, with the exception of
 *          exactly two elements.
 * https://oeis.org/A283226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   eval_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A283226 – recurrence implementation.
 *  a(n) = number of n×2 0‑1 arrays with exactly two cells allowed to have
 *  more than two 1‑neighbours (i.e. be surrounded by 1’s on all three
 *  possible sides).  The recurrence derived from the definition is
 *  computed by a dynamic‑programming algorithm that evaluates rows in
 *  order while keeping the number of exception cells ≤ 2.
 *  For n < 3 the required exception count cannot be satisfied, so
 *  a(n) = 0.  For n ≥ 3 the algorithm runs in O(n) time and uses
 *  only additions, bitwise operations and small constant loops.  The
 *  DP state is (previous‑two‑rows, previous‑row, exceptions‑used). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/eval_row.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* cannot have exactly two exception cells */

    /* dp[prev2][prev][k] – number of ways for the first i rows,
     * where the last two rows are prev2, prev and k exceptions used. */
    static seq_t dp[4][4][3];
    static seq_t ndp[4][4][3];

    /* initialise all states to 0 */
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < 3; ++k)
                dp[i][j][k] = 0;

    /* first two rows are unrestricted (no exception yet) */
    for (unsigned char r0 = 0; r0 < 4; ++r0)
        for (unsigned char r1 = 0; r1 < 4; ++r1)
            dp[r0][r1][0] = 1;

    /* add rows 2 .. n-1; each step evaluates row i-1 */
    for (idx_t i = 2; i < n; ++i) {
        /* reset ndp */
        for (int a = 0; a < 4; ++a)
            for (int b = 0; b < 4; ++b)
                for (int k = 0; k < 3; ++k)
                    ndp[a][b][k] = 0;

        for (unsigned char prev2 = 0; prev2 < 4; ++prev2)
            for (unsigned char prev = 0; prev < 4; ++prev)
                for (int k = 0; k < 3; ++k) {
                    seq_t ways = dp[prev2][prev][k];
                    if (!ways) continue;
                    for (unsigned char cur = 0; cur < 4; ++cur) {
                        int add_exc = eval_row(prev, prev2, cur);
                        int nk = k + add_exc;
                        if (nk > 2) continue;           /* too many exceptions */
                        ndp[prev][cur][nk] += ways;      /* shift window */
                    }
                }

        /* copy ndp back to dp for next iteration */
        for (int a = 0; a < 4; ++a)
            for (int b = 0; b < 4; ++b)
                for (int k = 0; k < 3; ++k)
                    dp[a][b][k] = ndp[a][b][k];
    }

    /* after the loop, only rows n-2 and n-1 remain un‑evaluated;
     * last row never contributes new exceptions.
     * Sum all states with exactly two exceptions. */
    seq_t result = 0;
    for (unsigned char prev = 0; prev < 4; ++prev)
        for (unsigned char cur = 0; cur < 4; ++cur)
            result += dp[prev][cur][2];

    return result;
}
