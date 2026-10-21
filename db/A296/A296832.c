/*
 * A296832  Number of nX6 0..1 arrays with each 1 adjacent to 0, 3 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is counted by a transfer‑matrix DP over rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   isValid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each state is the pair of the previous two rows (6‑bit masks). */
/* Transitions use precomputed validity of the middle row given its top and bottom rows. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isValid.h"

/* Cached transition data: for each (top,mid) pair, list of bottom rows that are valid. */
static unsigned char nxtCount[64][64];
static unsigned char nxtList[64][64][64];
static bool initialized = false;

/* Lazy initialization of transition tables. */
static void init_tables(void)
{
    if (initialized) return;
    for (uint8_t top = 0; top < 64; ++top) {
        for (uint8_t mid = 0; mid < 64; ++mid) {
            unsigned char c = 0;
            for (uint8_t bot = 0; bot < 64; ++bot) {
                if (isValid(top, mid, bot)) {
                    nxtList[top][mid][c++] = bot;
                }
            }
            nxtCount[top][mid] = c;
        }
    }
    initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    init_tables();

    /* Two 64×64 DP tables. */
    seq_t dp[2][64][64];
    int cur = 0, nxt = 1;

    /* Clear dp tables. */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j) {
            dp[0][i][j] = 0;
            dp[1][i][j] = 0;
        }

    /* Initial state: previous row is all zeros, current row is any pattern. */
    for (uint8_t r = 0; r < 64; ++r)
        dp[cur][0][r] = (seq_t)1;

    /* Process rows 2 .. n. */
    for (idx_t step = 2; step <= n; ++step) {
        /* Clear next table. */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[nxt][i][j] = 0;

        for (uint8_t top = 0; top < 64; ++top) {
            for (uint8_t mid = 0; mid < 64; ++mid) {
                seq_t ways = dp[cur][top][mid];
                if (ways == 0) continue;
                unsigned char *list = nxtList[top][mid];
                unsigned char count = nxtCount[top][mid];
                for (unsigned char k = 0; k < count; ++k) {
                    uint8_t bot = list[k];
                    dp[nxt][mid][bot] += ways;
                }
            }
        }
        /* Swap tables. */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Final row must satisfy condition with a zero bottom row. */
    seq_t ans = 0;
    for (uint8_t top = 0; top < 64; ++top) {
        for (uint8_t mid = 0; mid < 64; ++mid) {
            seq_t ways = dp[cur][top][mid];
            if (ways == 0) continue;
            if (isValid(top, mid, 0))
                ans += ways;
        }
    }
    return ans;
}
