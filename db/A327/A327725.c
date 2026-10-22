/*
 * A327725  Expansion of Product_{i>=1, j>=1} (1 + x^(i*prime(j))).
 * https://oeis.org/A327725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{i≥1, j≥1} (1 + x^(i*prime(j)))
 * Range:     u8 n <= 27, u16 n <= 65, u32 n <= 179, u64 n <= 542,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* convert index to size_t for array indexing */
    size_t N = (size_t)n;

    /* dp[t] will hold the coefficient of x^t after processing all items */
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;
    for (size_t i = 0; i <= N; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* prime sieve up to N */
    size_t limit = N;
    uint8_t *is_composite = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!is_composite) {
        free(dp);
        return (seq_t)0;
    }
    size_t primes_cnt = 0;
    uint32_t *primes = NULL;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            primes_cnt++;
            primes = (uint32_t *)realloc(primes, primes_cnt * sizeof(uint32_t));
            primes[primes_cnt - 1] = (uint32_t)i;
            if (i * i <= limit)
                for (size_t j = i * i; j <= limit; j += i)
                    is_composite[j] = 1;
        }
    }
    free(is_composite);

    /* 0‑1 knapsack over items with weight r * p for each prime p and 1 ≤ r ≤ N/p */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint32_t p = primes[pi];
        size_t max_r = N / p;
        for (size_t r = 1; r <= max_r; ++r) {
            size_t w = r * p;          /* weight of the current item */
            for (size_t t = N; t >= w; --t)
                dp[t] = dp[t] + dp[t - w];
        }
    }

    seq_t result = dp[N];
    free(dp);
    free(primes);
    return result;
}
