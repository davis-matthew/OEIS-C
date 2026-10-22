/*
 * A338458  Least number of heptagonal pyramidal numbers needed to represent
 *          n.
 * https://oeis.org/A338458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming for the minimal number of
 * heptagonal pyramidal numbers needed to sum to n.  */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n < 1)
        return 0;

    /* static state */
    static seq_t *dp = NULL;          /* minimal counts */
    static size_t dp_cap = 0;         /* allocated capacity */
    static size_t dp_len = 0;         /* highest computed index +1 */

    static idx_t *coins = NULL;        /* heptagonal pyramidal values */
    static size_t coin_cap = 0;
    static size_t coin_len = 0;

    /* Desired array size */
    size_t target_len = (size_t)n + 1;   /* indices 0..n */

    /* ---- ensure dp buffer size ---- */
    if (target_len > dp_cap) {
        size_t new_cap = target_len;
        if (new_cap < 64) new_cap = 64;          /* arbitrary minimal size */
        if (dp == NULL) {
            dp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        }
        dp_cap = new_cap;
    }

    /* ---- generate coins up to n ---- */
    if ((size_t)n > 0 && (size_t)n >= coin_len) {
        /* compute new coin count needed */
        idx_t k = (idx_t)coin_len + 1;   /* next k to try */
        size_t needed = coin_len;
        while (1) {
            /* value = k*(k+1)*(5*k-2)/6 */
            idx_t val = (k * (k + 1) * (5 * k - 2)) / 6;
            if (val > n)
                break;
            needed++;
            k++;
        }
        if (needed > coin_len) {
            /* grow coin array */
            size_t new_cap = needed;
            if (new_cap < 16) new_cap = 16;
            if (coins == NULL) {
                coins = (idx_t *)malloc(new_cap * sizeof(idx_t));
            } else {
                coins = (idx_t *)realloc(coins, new_cap * sizeof(idx_t));
            }
            /* fill new coin values */
            for (size_t i = coin_len; i < needed; ++i) {
                idx_t kk = (idx_t)(i + 1);
                coins[i] = (kk * (kk + 1) * (5 * kk - 2)) / 6;
            }
            coin_cap = new_cap;
            coin_len = needed;
        }
    }

    /* ---- compute DP for new indices ---- */
    for (size_t j = dp_len; j < target_len; ++j) {
        seq_t best = (seq_t)j;   /* worst case: all 1’s */
        for (size_t ci = 0; ci < coin_len; ++ci) {
            idx_t c = coins[ci];
            if (c > j)
                break;
            seq_t cand = dp[j - c] + 1;
            if (cand < best)
                best = cand;
        }
        dp[j] = best;
    }
    dp_len = target_len;

    return dp[n];
}
