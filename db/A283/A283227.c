/*
 * A283227  Number of n X 3 0..1 arrays with no 1 equal to more than two of
 *          its horizontal and vertical neighbors, with the exception of
 *          exactly two elements.
 * https://oeis.org/A283227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   violations_of_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of n×3 binary arrays in which exactly two 1‑cells
 * have more than two neighboring 1s (horizontal or vertical).
 * The function uses a dynamic programming algorithm that processes the
 * array one row at a time.  The algorithm runs in O(n) time and uses
 * O(1) additional memory beyond the state tables.
 */

#include <stdint.h>
#include "../utils/violations_of_row.h"

seq_t a(idx_t n)
{
    /* For n < 1 the problem is not defined, but we return 0 anyway. */
    if (n < 1)
        return 0;

    /* dp[prev][curr][v] : number of ways to reach the current row
     *   prev  – pattern of row i‑1 (0..7)
     *   curr  – pattern of row i   (0..7)
     *   v     – number of violations seen up to row i‑1 (0,1,2)
     */
    seq_t dp[8][8][3] = {{{0}}};
    seq_t next[8][8][3] = {{{0}}};

    /* Initialise with the first row; the dummy top row is 0. */
    for (int p1 = 0; p1 < 8; ++p1) {
        dp[0][p1][0] = 1;
    }

    /* Process rows 2 .. n.  Each iteration adds a new row. */
    for (idx_t row = 1; row < n; ++row) {
        /* reset next */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 3; ++k)
                    next[i][j][k] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                for (int v = 0; v <= 2; ++v) {
                    seq_t ways = dp[prev][curr][v];
                    if (ways == 0)
                        continue;

                    for (int next_row = 0; next_row < 8; ++next_row) {
                        int add_v = violations_of_row(curr, prev, next_row);
                        int v2 = v + add_v;
                        if (v2 > 2)
                            continue;
                        next[curr][next_row][v2] += ways;
                    }
                }
            }
        }

        /* copy next to dp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 3; ++k)
                    dp[i][j][k] = next[i][j][k];
    }

    /* After all rows have been added, evaluate the last row (curr) which
     * has no downward neighbour. Count arrays where total violations = 2. */
    seq_t result = 0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            for (int v = 0; v <= 2; ++v) {
                seq_t ways = dp[prev][curr][v];
                if (ways == 0)
                    continue;
                int final_v = v + violations_of_row(curr, prev, 0);
                if (final_v == 2)
                    result += ways;
            }
        }
    }

    return result;
}
