/*
 * A341465  Number of partitions of n into 6 distinct nonprime parts.
 * https://oeis.org/A341465
 *
 * Offset:    38    (a(38) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into 6 distinct nonprime parts
 * Range:     u8 n <= 70, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Defined only for n >= 38 – return 0 for smaller arguments. */
    if (n < (idx_t)38) return 0;

    /* Working with int is safe for n up to 10 000 (the typical limit). */
    int N = (int)n;

    /* 1. Sieve of Eratosthenes: isPrime[i] == 1 iff i is prime. */
    size_t sz = (size_t)N + 1;
    uint8_t *isPrime = (uint8_t *)calloc(sz, sizeof(uint8_t));
    if (!isPrime) return 0;          /* allocation failure fallback */

    for (size_t i = 2; i < sz; ++i) isPrime[i] = 1;  /* assume prime initially */
    if (sz > 2) {                    /* mark 0 and 1 as nonprime */
        isPrime[0] = isPrime[1] = 0;
    }
    for (size_t p = 2; p * p < sz; ++p) {
        if (isPrime[p]) {
            for (size_t q = p * p; q < sz; q += p) {
                isPrime[q] = 0;
            }
        }
    }

    /* 2. DP for counting 6‑element subsets of nonprime numbers that sum to n. */
    const int K = 6;                     /* number of parts required */
    size_t rows = (size_t)N + 1;
    size_t cols = K + 1;
    seq_t *dp = (seq_t *)calloc(rows * cols, sizeof(seq_t));
    if (!dp) {
        free(isPrime);
        return 0;                      /* allocation failure fallback */
    }
    dp[0] = 1;                          /* dp[0][0] = 1 */

    for (int v = 1; v <= N; ++v) {
        if (v == 1 || !isPrime[(size_t)v]) {    /* v is a nonprime number */
            for (int s = N; s >= v; --s) {
                size_t idxS   = (size_t)s * cols;
                size_t idxPrev = (size_t)(s - v) * cols;
                for (int k = K; k >= 1; --k) {
                    dp[idxS + k] += dp[idxPrev + (k - 1)];
                }
            }
        }
    }

    seq_t result = dp[(size_t)N * cols + K];

    /* 3. Clean up dynamically allocated memory. */
    free(isPrime);
    free(dp);

    return result;
}
