/*
 * A341123  Number of partitions of n into 5 prime powers (including 1).
 * https://oeis.org/A341123
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into exactly 5 prime powers
 *            (prime powers include 1 as 1 = p⁰)
 * Range:     u8 n <= 40, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* For indices below the offset the behaviour is undefined */
    if (n < 5)
        return (seq_t)0;

    /* --------------------------------------------------------------------- */
    /* 1. Generate all prime powers ≤ n together with the element 1.          */
    /* --------------------------------------------------------------------- */
    size_t prime_cnt = 0;
    uint64_t *prime_list = NULL;
    build_primes_upto(n, &prime_list, &prime_cnt);

    /* Over‑estimate the number of prime powers: each prime ≤ n gives
       at most log₂(n) + 1 ≤ 15 powers for n ≤ 2⁴⁰. 7 × 15 is generous.   */
    size_t val_cap = prime_cnt * 15 + 1;
    idx_t *vals = (idx_t *)malloc(val_cap * sizeof(idx_t));
    if (!vals) {
        free(prime_list);
        return (seq_t)0;
    }
    size_t valcnt = 0;
    vals[valcnt++] = (idx_t)1;               /* 1 is always allowed */

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = (idx_t)prime_list[pi];
        idx_t power = p;
        while (power <= n) {
            vals[valcnt++] = power;
            if (power > n / p)           /* stop to avoid overflow */
                break;
            power *= p;
        }
    }
    free(prime_list);

    /* --------------------------------------------------------------------- */
    /* 2. Unbounded‑knapsack DP: dp[sum][k] – ways to reach 'sum' with
       exactly k parts (k = 0..5).  The table is stored linearly.           */
    /* --------------------------------------------------------------------- */
    size_t rows = (size_t)n + 1;      /* sums 0 .. n */
    size_t cols = 6;                  /* parts 0 .. 5 */
    seq_t *dp = (seq_t *)calloc(rows * cols, sizeof(seq_t));
    if (!dp) {
        free(vals);
        return (seq_t)0;
    }
    dp[0] = (seq_t)1;                 /* zero sum with zero parts */

    for (size_t vi = 0; vi < valcnt; ++vi) {
        idx_t v = vals[vi];
        for (idx_t s = v; s <= n; ++s) {
            size_t base = (size_t)s * cols;
            size_t prev = (size_t)(s - v) * cols;
            for (int k = 1; k <= 5; ++k)
                dp[base + k] += dp[prev + k - 1];
        }
    }

    seq_t result = dp[(size_t)n * cols + 5];
    free(dp);
    free(vals);
    return result;
}
