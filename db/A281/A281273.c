/*
 * A281273  Expansion of Product_{j>=1} 1/(1 - x^(Sum_{i=1..j} prime(i))).
 * https://oeis.org/A281273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into non‑zero partial sums of
 *            primes (A281273). This is the usual coin‑change count where the
 *            coins are the partial sums S(k)=sum_{i=1..k} prime(i). The
 *            function recomputes the values from scratch for each call; the
 *            complexity is O(n · m) with m≈100 for n≤10000, w...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static bool is_prime_idx(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* allocate workspace for dynamic programming */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* generate coins = partial sums of primes up to n */
    idx_t *coins = NULL;
    size_t coins_cap = 0;
    size_t coins_len = 0;

    idx_t sum = 0;
    idx_t candidate = 2;
    while (sum <= n) {
        while (!is_prime_idx(candidate)) ++candidate;
        sum += candidate;
        if (sum <= n) {
            /* ensure capacity */
            if (coins_len == coins_cap) {
                size_t new_cap = coins_cap ? 2 * coins_cap : 8;
                idx_t *tmp = (idx_t *)realloc(coins, new_cap * sizeof(idx_t));
                if (!tmp) {
                    free(dp);
                    free(coins);
                    return 0;
                }
                coins = tmp;
                coins_cap = new_cap;
            }
            coins[coins_len++] = sum;
        }
        ++candidate;
    }

    /* dynamic programming: classic coin‑change */
    for (size_t ci = 0; ci < coins_len; ++ci) {
        idx_t c = coins[ci];
        for (idx_t i = c; i <= n; ++i) {
            dp[i] += dp[i - c];
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(coins);
    return result;
}
