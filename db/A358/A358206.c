/*
 * A358206  Number of ways of making change for n cents using coins of 1, 2,
 *          4, 10 and 20 cents.
 * https://oeis.org/A358206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1,2,4,10 and 20
 *            (coin‑change combinations)
 * Range:     u8 n <= 39, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define COINS_COUNT 5
static idx_t         coins[COINS_COUNT] = {1, 2, 4, 10, 20};
static seq_t        *dp      = NULL;  /* table of a(i) for i < dp_cap */
static size_t        dp_cap  = 0;      /* next index to be computed (size of dp) */

/* Assemble the dynamic programming table up to at least n and return a(n). */
seq_t a(idx_t n)
{
    /* If we haven't allocated anything yet, or need more space, reallocate. */
    if (dp_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* out of memory: abort by returning 0 (still a valid seq_t value). */
            if (dp) free(dp);
            dp      = NULL;
            dp_cap  = 0;
            return (seq_t)0;
        }
        dp     = tmp;
        dp_cap = new_cap;

        /* Initialise and recompute the whole table.  Because the coins set is fixed,
           re‑computing from scratch for each request is acceptable and keeps the
           code simple and correct. */
        dp[0] = (seq_t)1;                    /* a(0) = 1 */
        for (size_t i = 1; i < dp_cap; ++i)
            dp[i] = (seq_t)0;

        for (int c = 0; c < COINS_COUNT; ++c) {
            idx_t coin = coins[c];
            for (idx_t i = coin; i < dp_cap; ++i) {
                dp[i] += dp[i - coin];
            }
        }
    }

    return dp[n];
}
