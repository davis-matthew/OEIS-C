/*
 * A338494  Least number of pentagonal pyramidal numbers needed to represent
 *          n.
 * https://oeis.org/A338494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least number of pentagonal pyramidal numbers needed to
 *            represent n, where P(k) = k*k*(k+1)/2
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence has a(1)=1 because P(1)=1.  For any n>=1 we
       can represent n using at most n terms of value 1, so the
       answer always fits in 64-bit.  We do all arithmetic in
       64 bit integers and cast the result to seq_t. */
    if (n == 0) {
        return 0;
    }

    size_t N = (size_t)n;

    /* Generate all pentagonal pyramidal numbers <= n. */
    uint64_t *coins = (uint64_t *)malloc(8 * sizeof(uint64_t));
    size_t cap = 8, len = 0;
    for (uint64_t k = 1;; ++k) {
        uint64_t val = k * k * (k + 1) / 2;
        if (val > (uint64_t)n) {
            break;
        }
        if (len == cap) {
            size_t old_cap = cap;
            cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(coins, cap * sizeof(uint64_t));
            if (!tmp) {
                free(coins);
                return 0; /* allocation failure - unlikely for n<=10000 */
            }
            coins = tmp;
        }
        coins[len++] = val;
    }

    /* Dynamic programming: dp[i] = minimal number of coins summing to i. */
    uint64_t *dp = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    if (!dp) {
        free(coins);
        return 0;
    }
    for (size_t i = 1; i <= N; ++i) {
        dp[i] = N + 1;          /* upper bound on answer */
    }
    dp[0] = 0;

    for (size_t i = 1; i <= N; ++i) {
        for (size_t c = 0; c < len; ++c) {
            uint64_t coin = coins[c];
            if (coin > i) {
                break;
            }
            uint64_t cand = dp[i - coin] + 1;
            if (cand < dp[i]) {
                dp[i] = cand;
            }
        }
    }

    seq_t result = (seq_t)dp[N];

    free(dp);
    free(coins);
    return result;
}
