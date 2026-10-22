/*
 * A341462  Number of partitions of n into 4 distinct nonprime parts.
 * https://oeis.org/A341462
 *
 * Offset:    19    (a(19) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 4 distinct nonprime parts
 * Range:     u8 n <= 66, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Count partitions of n into 4 distinct nonprime parts */
seq_t a(idx_t n)
{
    /* Sieve of Eratosthenes to find primes up to n */
    bool *is_prime = (bool *)calloc((size_t)(n + 1), sizeof(bool));
    if (!is_prime) return 0;
    for (idx_t i = 0; i <= n; ++i) is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;
    for (idx_t p = 2; (p * p) <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t mult = p * p; mult <= n; mult += p)
                is_prime[mult] = false;
        }
    }

    /* Gather all nonprime numbers up to n (including 1) */
    uint64_t *nonprimes = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!nonprimes) {
        free(is_prime);
        return 0;
    }
    size_t npcnt = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1 || !is_prime[i]) {
            nonprimes[npcnt++] = i;
        }
    }

    /* Dynamic programming: dp[k][s] = #ways to pick k distinct nonprimes summing to s */
    seq_t *dp[5];
    for (int k = 0; k < 5; ++k) {
        dp[k] = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!dp[k]) {
            for (int j = 0; j < k; ++j) free(dp[j]);
            free(nonprimes);
            free(is_prime);
            return 0;
        }
    }
    dp[0][0] = (seq_t)1;  /* base case: one way to sum to 0 with 0 parts */

    /* Update DP for each nonprime number */
    for (size_t idx = 0; idx < npcnt; ++idx) {
        idx_t x = (idx_t)nonprimes[idx];
        for (int k = 4; k >= 1; --k) {
            for (idx_t s = n; s >= x; --s) {
                dp[k][s] += dp[k - 1][s - x];
            }
        }
    }

    seq_t result = dp[4][n];

    /* Free allocated memory */
    for (int k = 0; k < 5; ++k) free(dp[k]);
    free(nonprimes);
    free(is_prime);

    return result;
}
