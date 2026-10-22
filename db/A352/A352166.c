/*
 * A352166  Number of partitions of n into distinct odd prime powers (1
 *          included).
 * https://oeis.org/A352166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct odd prime powers
 *            (1 included). Computation via subset‑sum DP over the set S = {1}
 *            ∪ {p^k : p odd prime, p^k ≤ n}
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Returns a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    /* Allocate dynamic array for DP counts */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;                       /* out of memory */

    /* initialise to zero */
    for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = 1;

    /* Simple sieve to get primes up to n (2 is generated but will be skipped later). */
    size_t limit = (size_t)n;
    bool *is_composite = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_composite) { free(dp); return 0; }

    /* dynamic array for primes */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0, prime_cap = 0;

    for (uint64_t p = 2; p <= limit; ++p) {
        if (!is_composite[p]) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                if (!tmp) { free(is_composite); free(primes); free(dp); return 0; }
                primes = tmp;
            }
            primes[prime_cnt++] = p;
            if (p <= limit / p) {
                for (uint64_t mult = p * p; mult <= limit; mult += p)
                    is_composite[(size_t)mult] = true;
            }
        }
    }
    free(is_composite);

    /* Process part 1 (used at most once) */
    for (idx_t i = n; i >= (idx_t)1; --i)
        dp[i] += dp[i - 1];

    /* Process odd prime powers */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        if (p == 2) continue;                /* only odd primes allowed */

        seq_t power = (seq_t)p;
        while (power <= n) {
            idx_t val = (idx_t)power;
            for (idx_t i = n; i >= val; --i)
                dp[i] += dp[i - val];
            /* next power, guard against overflow when power > n/p but power <= n. */
            if (power > n / (seq_t)p) break;  /* would exceed n on next iteration */
            power *= (seq_t)p;
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(primes);
    return result;
}
