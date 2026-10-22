/*
 * A341949  Number of partitions of n into 6 primes (counting 1 as a prime).
 * https://oeis.org/A341949
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 6 primes (counting 1 as a
 *            prime). For n < 6 the value is undefined, but the function
 *            returns 0 for simplicity
 * Range:     u8 n <= 61, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 6) return 0;

    size_t N = (size_t)n;
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;
    size_t cap_needed = N + 1;

    if (!dp || dp_cap < cap_needed) {
#ifdef __cplusplus
        delete[] dp;
        dp = new seq_t[7 * cap_needed];
#else
        free(dp);
        dp = (seq_t*)malloc(7 * cap_needed * sizeof(seq_t));
#endif
        dp_cap = cap_needed;
    }

    /* Clear DP table */
    memset(dp, 0, 7 * cap_needed * sizeof(seq_t));
    /* Base: dp[0][0] = 1 */
    dp[0] = 1;

    /* Coin 1 (counted as a prime) */
    for (size_t s = 1; s <= N; ++s) {
        for (size_t i = 1; i <= 6; ++i) {
            dp[i * cap_needed + s] += dp[(i - 1) * cap_needed + (s - 1)];
        }
    }

    /* Get all primes up to N */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(N, &primes, &prime_cnt);

    /* Process each prime (>=2) */
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        size_t p = (size_t)primes[idx];
        for (size_t s = p; s <= N; ++s) {
            for (size_t i = 1; i <= 6; ++i) {
                dp[i * cap_needed + s] += dp[(i - 1) * cap_needed + (s - p)];
            }
        }
    }

    free(primes);

    return dp[6 * cap_needed + N];
}
