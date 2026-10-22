/*
 * A358637  Number of partitions of n into at most 4 distinct prime powers
 *          (including 1).
 * https://oeis.org/A358637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at most 4 distinct prime
 *            powers (including 1)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   safe_mul_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/safe_mul_uint64.h"

seq_t a(idx_t n) {
    /* static cache for prime powers */
    static idx_t cached_max_n = 0;
    static uint64_t *prime_powers = NULL;
    static size_t prime_power_count = 0;

    /* Compute prime powers up to n (if needed) */
    if (n > cached_max_n) {
        /* free old cache if exists */
        if (prime_powers) free(prime_powers);

        /* Sieve primes up to n */
        size_t limit = (size_t)n;
        bool *is_composite = (bool*)calloc(limit + 1, sizeof(bool));
        if (!is_composite) return 0;  /* out of memory, return 0 as fallback */

        uint32_t *primes = (uint32_t*)malloc(sizeof(uint32_t) * (limit + 1));
        size_t prime_cnt = 0;
        for (size_t i = 2; i <= limit; ++i) {
            if (!is_composite[i]) {
                primes[prime_cnt++] = (uint32_t)i;
                if (i <= limit / i) {
                    for (size_t j = i * i; j <= limit; j += i)
                        is_composite[j] = true;
                }
            }
        }

        /* Allocate space for prime powers (worst case n+1 entries) */
        uint64_t *pp = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
        size_t pp_cnt = 0;
        if (n >= 1) {
            pp[pp_cnt++] = 1;  /* include 1 as a prime power */
        }
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            uint64_t p = primes[pi];
            uint64_t val = p;
            while (val <= n) {
                pp[pp_cnt++] = val;
                /* prepare next power, stop to avoid overflow */
                val = safe_mul_uint64(val, p, n);
                if (val > n) break;
            }
        }

        free(is_composite);
        free(primes);

        prime_powers = pp;
        prime_power_count = pp_cnt;
        cached_max_n = n;
    }

    /* DP table: dp[k][s] for k=0..4, sum s=0..n */
    size_t dp_rows = 5;
    size_t dp_cols = (size_t)n + 1;
    seq_t *dp = (seq_t*)calloc(dp_rows * dp_cols, sizeof(seq_t));
    if (!dp) return 0;  /* out of memory */

    /* Helper to index dp */
    #define DP(k, s) dp[(k)*(dp_cols) + (s)]

    DP(0, 0) = 1;

    /* Iterate through each prime power <= n */
    for (size_t i = 0; i < prime_power_count; ++i) {
        uint64_t val = prime_powers[i];
        if (val > n) break;
        /* Update DP in reverse order to enforce distinctness */
        for (int k = 4; k >= 1; --k) {
            for (size_t s = dp_cols - 1; s > val; --s) {
                DP(k, s) += DP(k - 1, s - (size_t)val);
            }
            /* handle s == val separately because of size_t and unsigned loop */
            DP(k, val) += DP(k - 1, 0);
        }
    }

    /* Sum counts for partitions with up to 4 parts */
    seq_t result = DP(0, n);
    result += DP(1, n);
    result += DP(2, n);
    result += DP(3, n);
    result += DP(4, n);

    free(dp);
    return result;
}
