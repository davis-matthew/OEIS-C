/*
 * A341947  Number of partitions of n into 4 primes (counting 1 as a prime).
 * https://oeis.org/A341947
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 4 primes counting 1 as a
 *            prime. The implementation uses a simple coin–change dynamic
 *            programming that counts unordered combinations (repetition of
 *            primes is allowed). For each index n ≥ 4 it returns the exact
 *            value in seq_t. (Storing primes up to n lazily and...
 * Range:     u8 n <= 127, u16 n <= 1613, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 4) /* below the OEIS offset – defined but unused */
        return (seq_t)0;

    /* ------------------------------------------------------------
       1. Lazily build the set of primes up to n (treating 1 as a prime).
       ------------------------------------------------------------ */
    static idx_t sieve_limit = 0;
    static uint8_t *is_prime = NULL;   /* 1 = prime, 0 = composite / unmarked */
    static idx_t *prime_list = NULL;   /* list of primes including 1 */
    static idx_t prime_cnt = 0;

    if (n > sieve_limit) {
        free(is_prime);
        free(prime_list);

        /* allocate and init */
        is_prime = (uint8_t *)calloc((size_t)(n + 1), sizeof(uint8_t));
        if (!is_prime) return (seq_t)0;
        prime_list = (idx_t *)malloc((size_t)(n + 2) * sizeof(idx_t));
        if (!prime_list) {
            free(is_prime);
            return (seq_t)0;
        }

        /* 0 is not prime; 1 is treated as prime; all others initially prime */
        is_prime[0] = 0;
        is_prime[1] = 1;
        for (idx_t i = 2; i <= n; ++i) is_prime[i] = 1;

        /* classic Eratosthenes sieve */
        for (idx_t i = 2; i * i <= n; ++i)
            if (is_prime[i])
                for (idx_t j = i * i; j <= n; j += i)
                    is_prime[j] = 0;

        /* build the prime list (with 1 included) */
        prime_cnt = 0;
        for (idx_t i = 1; i <= n; ++i)
            if (is_prime[i])
                prime_list[prime_cnt++] = i;

        sieve_limit = n;
    }

    /* ------------------------------------------------------------
       2. Dynamic programming: dp[k][s] = #ways to write s as sum
          of k primes (unordered, repetitions allowed).
       ------------------------------------------------------------ */
    seq_t *dp[5];
    for (int k = 0; k <= 4; ++k) {
        dp[k] = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!dp[k]) {
            for (int t = 0; t < k; ++t)
                free(dp[t]);
            return (seq_t)0;
        }
    }
    dp[0][0] = (seq_t)1; /* one way to make zero with zero primes */

    /* iterate over each prime (coin type) */
    for (idx_t idx = 0; idx < prime_cnt; ++idx) {
        idx_t p = prime_list[idx];
        if (p > n) break;
        for (idx_t sum = p; sum <= n; ++sum) {
            for (int k = 1; k <= 4; ++k) {
                dp[k][sum] += dp[k - 1][sum - p];
            }
        }
    }

    seq_t result = dp[4][n];

    /* free DP memory */
    for (int k = 0; k <= 4; ++k)
        free(dp[k]);

    return result;
}
