/*
 * A341127  Number of partitions of n into 9 prime powers (including 1).
 * https://oeis.org/A341127
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the number of unordered partitions of n into
   exactly nine prime powers (including 1).  The classic coin‑change DP
   with an extra dimension for the number of parts is used.  The list
   of coin values is all prime powers ≤ n together with the value 1. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n)
{
    /* Spec: a(9) = 1, …, as given in the OEIS entry. */
    if (n < 9)
        return (seq_t)0;          /* offset behaviour is irrelevant */

    /* ---------- 1. Generate all primes ≤ n ---------- */
    size_t nn = (size_t)n;
    bool *is_prime = (bool *)malloc((nn + 1) * sizeof(bool));
    if (!is_prime)
        return (seq_t)0;
    memset(is_prime, true, (nn + 1) * sizeof(bool));
    if (nn >= 0) is_prime[0] = false;
    if (nn >= 1) is_prime[1] = false;
    for (size_t p = 2; p * p <= nn; ++p)
    {
        if (is_prime[p])
        {
            for (size_t m = p * p; m <= nn; m += p)
                is_prime[m] = false;
        }
    }

    /* ---------- 2. Enumerate all prime powers ≤ n ---------- */
    size_t pc_sz = nn + 1;                /* rough upper bound */
    uint64_t *prime_powers = (uint64_t *)malloc(pc_sz * sizeof(uint64_t));
    if (!prime_powers)
    {
        free(is_prime);
        return (seq_t)0;
    }
    size_t pc_cnt = 0;

    for (size_t p = 2; p <= nn; ++p)
    {
        if (!is_prime[p])
            continue;
        uint64_t power = (uint64_t)p;
        while (power <= (uint64_t)n)
        {
            prime_powers[pc_cnt++] = power;
            if (power > (uint64_t)n / p)
                break;                   /* avoid overflow */
            power *= p;
        }
    }

    /* include the coin value 1 */
    if (nn >= 1 && pc_cnt < pc_sz)
        prime_powers[pc_cnt++] = 1ULL;

    free(is_prime);

    /* ---------- 3. DP for combinations with exactly 9 parts ---------- */
    const size_t K = 9;
    const size_t K1 = K + 1;
    size_t dp_rows = nn + 1;
    size_t dp_size = dp_rows * K1;

    seq_t *dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
    if (!dp)
    {
        free(prime_powers);
        return (seq_t)0;
    }
    dp[0] = (seq_t)1;                     /* one way to make 0 with 0 parts */

    for (size_t idx = 0; idx < pc_cnt; ++idx)
    {
        size_t coin = (size_t)prime_powers[idx];
        for (size_t s = coin; s <= nn; ++s)
        {
            size_t base  = s * K1;
            size_t prev  = (s - coin) * K1;
            for (size_t k = 1; k <= K; ++k)
                dp[base + k] += dp[prev + (k - 1)];
        }
    }

    seq_t result = dp[nn * K1 + K];

    free(dp);
    free(prime_powers);

    return result;
}
