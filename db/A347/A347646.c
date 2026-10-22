/*
 * A347646  Number of partitions of n into at most 5 prime powers (including
 *          1).
 * https://oeis.org/A347646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at most 5 prime powers
 *            (including 1)
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static int cmp_idx_t(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    return (va < vb) - (va > vb);
}

seq_t a(idx_t n)
{
    /* Edge case: negative index is out of bounds; return 0 */
    if ((int64_t)n < 0) return 0;

    /* 1. Generate list of allowed parts: 1 and all prime powers ≤ n */
    /* Sieve to find primes up to n */
    idx_t limit = n;
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return 0;
    memset(is_prime, true, (limit + 1) * sizeof(bool));
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* Collect primes */
    idx_t primes_cap = limit + 1;
    idx_t *primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    if (!primes) { free(is_prime); return 0; }
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[primes_cnt++] = i;

    /* Build parts array: start with 1 */
    idx_t parts_cap = limit + 1;           /* maximum possible parts */
    idx_t *parts = (idx_t *)malloc(parts_cap * sizeof(idx_t));
    if (!parts) { free(is_prime); free(primes); return 0; }
    size_t parts_cnt = 0;
    parts[parts_cnt++] = 1;   /* 1 is a prime power for this sequence */

    /* Add prime powers > 1 */
    for (size_t pi = 0; pi < primes_cnt; ++pi)
    {
        idx_t p = primes[pi];
        idx_t val = p;
        while (val <= n)
        {
            parts[parts_cnt++] = val;
            if (val > n / p) break;  /* avoid overflow */
            val *= p;
        }
    }
    free(is_prime);
    free(primes);

    /* Sort parts to guarantee non‑decreasing order in DP */
    qsort(parts, parts_cnt, sizeof(idx_t), cmp_idx_t);

    /* 2. Dynamic programming for partitions with at most 5 parts */
    size_t dp_rows = (n + 1) * 6;              /* 6 columns for k = 0..5 */
    seq_t *dp = (seq_t *)malloc(dp_rows * sizeof(seq_t));
    if (!dp) { free(parts); return 0; }
    memset(dp, 0, dp_rows * sizeof(seq_t));
    dp[0] = 1;                                 /* dp[0][0] = 1 */

    for (size_t pi = 0; pi < parts_cnt; ++pi)
    {
        idx_t part = parts[pi];
        for (idx_t sum = part; sum <= n; ++sum)
        {
            size_t base      = sum * 6;
            size_t prev_base = (sum - part) * 6;
            for (int k = 1; k <= 5; ++k)
                dp[base + k] += dp[prev_base + (k - 1)];
        }
    }
    free(parts);

    /* 3. Sum over all k = 0..5 to get partitions with at most 5 parts */
    seq_t ans = 0;
    size_t base = n * 6;
    for (int k = 0; k <= 5; ++k)
        ans += dp[base + k];

    free(dp);
    return ans;
}
