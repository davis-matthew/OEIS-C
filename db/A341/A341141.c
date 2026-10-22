/*
 * A341141  Number of partitions of n into 4 distinct prime powers (including
 *          1).
 * https://oeis.org/A341141
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   A341141 = number of partitions of n into 4 distinct prime powers
 *            (including 1)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   int_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is written to compile with C11 and C++17. */
/* It uses only the allowed operations and utilities. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>   /* for uint8_t, etc. */
#include "../utils/int_cmp.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Behaviour below the offset is irrelevant; return 0 for n < 10 */
    if (n < 10)
        return 0;

    /* Work within 32‑bit size_t domain for arrays (n ≤ 10000 for tests) */
    size_t N = (size_t)n;

    /* ------------------------------------------------------------
       1. Generate all primes ≤ N with a simple sieve
       ------------------------------------------------------------ */
    uint8_t *is_prime = (uint8_t *)calloc(N + 1, sizeof(uint8_t));
    if (!is_prime)
        return 0;   /* out of memory */
    for (size_t i = 2; i <= N; ++i)
        is_prime[i] = 1;
    for (size_t p = 2; p * p <= N; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= N; q += p)
                is_prime[q] = 0;
        }
    }

    /* Count primes and store them */
    size_t prime_cnt = 0;
    for (size_t i = 2; i <= N; ++i)
        if (is_prime[i])
            ++prime_cnt;

    int *primes = (int *)malloc(prime_cnt * sizeof(int));
    if (!primes) {
        free(is_prime);
        return 0;
    }
    size_t pc = 0;
    for (size_t i = 2; i <= N; ++i)
        if (is_prime[i])
            primes[pc++] = (int)i;
    free(is_prime);   /* primes now stored */

    /* ------------------------------------------------------------
       2. Build list of distinct prime powers ≤ N (including 1)
       ------------------------------------------------------------ */
    /* Expected size is moderate; reserve a generous capacity */
    size_t pw_cap = prime_cnt * 15 + 10;
    int *powers = (int *)malloc(pw_cap * sizeof(int));
    if (!powers) {
        free(primes);
        return 0;
    }
    size_t pw_cnt = 0;

    /* Include 1 as a prime power */
    powers[pw_cnt++] = 1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t val = (uint64_t)primes[pi];
        while (val <= N) {
            if (pw_cnt >= pw_cap) {
                pw_cap *= 2;
                int *tmp = (int *)realloc(powers, pw_cap * sizeof(int));
                if (!tmp) {
                    free(primes);
                    free(powers);
                    return 0;
                }
                powers = tmp;
            }
            powers[pw_cnt++] = (int)val;

            /* Prevent overflow when multiplying */
            if (val > N / primes[pi])
                break;
            val *= primes[pi];
        }
    }
    free(primes);
    qsort(powers, pw_cnt, sizeof(int), int_cmp);

    /* ------------------------------------------------------------
       3. DP: count 4‑element subsets summing to each value
       ------------------------------------------------------------ */
    seq_t **dp = (seq_t **)malloc(5 * sizeof(seq_t *));
    if (!dp) {
        free(powers);
        return 0;
    }
    for (int k = 0; k <= 4; ++k) {
        dp[k] = (seq_t *)calloc(N + 1, sizeof(seq_t));
        if (!dp[k]) {
            for (int j = 0; j < k; ++j)
                free(dp[j]);
            free(dp);
            free(powers);
            return 0;
        }
    }
    dp[0][0] = 1;   /* base case: one way to choose zero numbers summing to 0 */

    for (size_t idx = 0; idx < pw_cnt; ++idx) {
        int val = powers[idx];
        if ((size_t)val > N)
            break;   /* remaining powers are larger */

        for (int k = 4; k >= 1; --k) {
            seq_t *dp_k   = dp[k];
            seq_t *dp_km  = dp[k - 1];
            for (int s = (int)N; s >= val; --s) {
                dp_k[s] += dp_km[s - val];
            }
        }
    }

    seq_t result = dp[4][N];

    /* ------------------------------------------------------------
       4. Clean up
       ------------------------------------------------------------ */
    for (int k = 0; k <= 4; ++k)
        free(dp[k]);
    free(dp);
    free(powers);

    return result;
}
