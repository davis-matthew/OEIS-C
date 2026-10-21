/*
 * A297759  Number of nX5 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2, 3 or 4 neighboring 1s.
 * https://oeis.org/A297759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   transition_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/transition_valid.h"

/* This implementation counts n×5 binary arrays where each 1
   has between 2 and 4 adjacent 1s in the 6‐neighbourhood
   composed of horizontal neighbours (left/right) and
   the four diagonals (up‑left, up‑right, down‑left, down‑right).
   The DP keeps the last two rows and transitions to a next row,
   checking the middle row’s validity.  A dummy zero row is used on
   the top and bottom to avoid special boundary tests.  The routine
   works for any integer size of seq_t and memoises the transition
   table.  The result is returned as seq_t. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* below offset – not required */

    /* Pre‑compute transition masks once */
    static bool ready = false;
    static uint32_t next_mask[32][32];
    if (!ready) {
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                uint32_t m = 0;
                for (int c = 0; c < 32; ++c)
                    if (transition_valid((uint8_t)a, (uint8_t)b, (uint8_t)c))
                        m |= (1u << c);
                next_mask[a][b] = m;
            }
        }
        ready = true;
    }

    /* DP tables: dp[a][b] = number of ways up to current rows a,b */
    seq_t dp[32][32];
    seq_t dp_next[32][32];

    /* initialise: two dummy zero rows at the top */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;
    dp[0][0] = 1;   /* rows -1 and 0 are zeros */

    /* Process n real rows plus one dummy bottom row */
    for (idx_t row = 0; row <= n; ++row) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = 0;

        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                seq_t ways = dp[a][b];
                if (ways == 0) continue;

                if (row == n) {                  /* last dummy bottom row */
                    if (next_mask[a][b] & 1u)   /* c must be zero */
                        dp_next[b][0] += ways;
                } else {
                    uint32_t m = next_mask[a][b];
                    for (int c = 0; c < 32; ++c)
                        if (m & (1u << c))
                            dp_next[b][c] += ways;
                }
            }
        }
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* After the bottom dummy row, the last seen row is zero. */
    seq_t result = 0;
    for (int a = 0; a < 32; ++a)
        result += dp[a][0];
    return result;
}
