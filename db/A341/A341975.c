/*
 * A341975  Number of partitions of n into 4 distinct primes (counting 1 as a
 *          prime).
 * https://oeis.org/A341975
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 4 distinct primes (the
 *            integer 1 is treated as a prime)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Memory for sieve: use uint8_t to avoid C++ bool->void* conversion. */
    size_t limit = (size_t)n;
    uint8_t *sieve = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!sieve) return 0;

    /* Allocate space for the list of primes (include 1 explicitly). */
    size_t primes_est = limit + 1;          /* upper bound on number of primes ≤ n */
    uint64_t *primes = (uint64_t *)malloc(primes_est * sizeof(uint64_t));
    if (!primes) { free(sieve); return 0; }

    size_t primes_cnt = 0;
    primes[primes_cnt++] = 1;               /* 1 is treated as a prime */

    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[(size_t)i]) {
            if (primes_cnt == primes_est) {
                primes_est <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_est * sizeof(uint64_t));
                if (!tmp) { free(sieve); free(primes); return 0; }
                primes = tmp;
            }
            primes[primes_cnt++] = (uint64_t)i;

            if ((uint64_t)i * (uint64_t)i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    sieve[(size_t)j] = 1;
            }
        }
    }
    free(sieve);

    /* DP table: dp[k][s] – ways to partition s using k distinct primes. */
    size_t rows = limit + 1;
    size_t dp_size = rows * 5;                /* k = 0..4 */
    seq_t *dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
    if (!dp) { free(primes); return 0; }

    dp[0] = 1;                                /* base: 0 sum with 0 primes */

    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        if (p > n) break;
        for (idx_t s = (idx_t)n; s >= p; --s) {
            size_t s_idx = (size_t)s;
            for (size_t k = 4; k > 0; --k) {
                dp[k * rows + s_idx] += dp[(k - 1) * rows + (s_idx - (size_t)p)];
            }
        }
    }

    seq_t result = dp[4 * rows + (size_t)n];
    free(dp);
    free(primes);
    return result;
}
