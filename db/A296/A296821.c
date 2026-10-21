/*
 * A296821  Number of n X 2 0..1 arrays with each 1 adjacent to 2, 3 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   curr_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/curr_valid.h"

/* Sequence A296821: Number of n×2 0/1 arrays in which each 1 has
 * between 2 and 4 king‑move neighboring 1s.  The function is
 * implemented by a linear DP over the rows. */

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;

    if (n == 1u)
        return 1;                       /* only the all‑zero row */

    /* dp[cur][prev][curr] – number of ways to reach a state where
     * the last two processed rows are prev (row r‑1) and curr (row r). */
    static seq_t dp[2][4][4];
    unsigned cur = 0, nxt = 1;

    /* Initialise all entries to 0 for safety. */
    for (unsigned p = 0; p < 4; ++p)
        for (unsigned c = 0; c < 4; ++c)
            dp[cur][p][c] = 0;

    /* First row: previous row is all zeros.  Every first row is admissible
     * at this point – it will be checked when the second row is chosen. */
    for (unsigned first = 0; first < 4; ++first)
        dp[cur][0u][first] = 1;

    /* Process rows 2 … n.  At step 'row', 'next' represents the current
     * row r; we validate the previous row (r‑1) using its two neighbours. */
    for (idx_t row = 2; row <= n; ++row) {
        for (unsigned p = 0; p < 4; ++p)
            for (unsigned c = 0; c < 4; ++c)
                dp[nxt][p][c] = 0;          /* clear next layer */

        for (unsigned prev = 0; prev < 4; ++prev) {
            for (unsigned curr = 0; curr < 4; ++curr) {
                seq_t ways = dp[cur][prev][curr];
                if (!ways)
                    continue;
                for (unsigned next = 0; next < 4; ++next) {
                    if (curr_valid(prev, curr, next))
                        dp[nxt][curr][next] += ways;
                }
            }
        }
        /* Swap layers. */
        unsigned tmp = cur; cur = nxt; nxt = tmp;
    }

    /* After the loop, cur holds the last processed layer:
     * prev = row n‑1, curr = row n.  We must check that the last row
     * satisfies its neighbour requirements with a trailing zero row. */
    seq_t total = 0;
    for (unsigned prev = 0; prev < 4; ++prev) {
        for (unsigned curr = 0; curr < 4; ++curr) {
            seq_t ways = dp[cur][prev][curr];
            if (!ways)
                continue;
            if (curr_valid(prev, curr, 0u))
                total += ways;
        }
    }
    return total;
}
