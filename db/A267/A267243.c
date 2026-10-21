/*
 * A267243  Number of n X 6 binary arrays with row sums nondecreasing and
 *          columns lexicographically nondecreasing.
 * https://oeis.org/A267243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   popcnt6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A267243:
   Number of n×6 binary arrays with nondecreasing row sums
   and lexicographically nondecreasing columns.
   Dynamic programming over rows: dp[mask][last_sum] counts
   partial arrays.  mask is a 5‑bit field indicating which
   adjacent column pairs are still unresolved (bits equal so far).
   Complexity: O(n·32·7·64) ≈ O(n). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/popcnt6.h"

seq_t a(idx_t n)
{
    /* DP arrays: two layers for current/next row,
       32 possible masks, 7 possible last sums (0..6). */
    seq_t dp[2][32][7];
    unsigned i, j, p, cur, next;
    /* initialise */
    for (i = 0; i < 32; ++i)
        for (j = 0; j < 7; ++j)
            dp[0][i][j] = 0;
    dp[0][31][0] = 1;          /* all adjacent pairs unresolved, last sum 0 */

    cur = 0;
    next = 1;
    for (idx_t r = 0; r < n; ++r)
    {
        /* clear next layer */
        for (i = 0; i < 32; ++i)
            for (j = 0; j < 7; ++j)
                dp[next][i][j] = 0;

        for (i = 0; i < 32; ++i)          /* mask */
        {
            for (j = 0; j < 7; ++j)      /* last_sum */
            {
                seq_t ways = dp[cur][i][j];
                if (!ways)
                    continue;             /* skip zero counts */

                for (p = 0; p < 64; ++p)   /* 6‑bit row pattern */
                {
                    uint8_t mask = i;
                    uint8_t new_mask = mask;
                    int ok = 1;

                    /* update mask based on unresolved pairs */
                    for (int k = 0; k < 5; ++k)
                    {
                        if ((mask >> k) & 1u)
                        {
                            int a = (p >> k) & 1;
                            int b = (p >> (k + 1)) & 1;
                            if (a > b) {               /* violates lex order */
                                ok = 0;
                                break;
                            }
                            if (a < b)                  /* pair resolved */
                                new_mask &= ~(1u << k);
                        }
                    }
                    if (!ok)
                        continue;

                    uint8_t sum = popcnt6(p);
                    if (sum < j)                /* row sums must be nondecreasing */
                        continue;

                    dp[next][new_mask][sum] += ways;
                }
            }
        }
        /* swap layers */
        unsigned tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum all ending states */
    seq_t total = 0;
    for (i = 0; i < 32; ++i)
        for (j = 0; j < 7; ++j)
            total += dp[cur][i][j];

    return total;
}
