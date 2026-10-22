/*
 * A352165  Number of partitions of n into odd prime powers (1 included).
 * https://oeis.org/A352165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into odd prime powers (1
 *            included)
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
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
    /* Handle the trivial case of n = 0 */
    if (n == 0) return 1;

    size_t max_n = (size_t)n;

    /* Allocate array to hold the part values */
    size_t parts_cap = max_n + 1;
    uint64_t *parts = (uint64_t *)malloc(parts_cap * sizeof(uint64_t));
    if (!parts) return 0;            /* allocation failed */
    size_t parts_cnt = 0;
    parts[parts_cnt++] = 1ULL;       /* prime power 1 is always allowed */

    /* Simple sieve of Eratosthenes to find odd primes up to n */
    unsigned char *is_prime =
        (unsigned char *)malloc((max_n + 1) * sizeof(unsigned char));
    if (!is_prime) { free(parts); return 0; }
    for (size_t i = 0; i <= max_n; ++i) is_prime[i] = 1;
    if (max_n >= 0) is_prime[0] = 0;
    if (max_n >= 1) is_prime[1] = 0;
    for (size_t i = 2; i * i <= max_n; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= max_n; j += i)
                is_prime[j] = 0;

    /* Collect odd prime powers up to n */
    for (uint64_t p = 3; p <= max_n; p += 2) {
        if (!is_prime[(size_t)p]) continue;
        uint64_t val = p;
        while (val <= max_n) {
            parts[parts_cnt++] = val;
            if (val > max_n / p) break;  /* prevent overflow */
            val *= p;
        }
    }
    free(is_prime);

    /* Dynamic programming array to count partitions */
    seq_t *dp = (seq_t *)calloc(max_n + 1, sizeof(seq_t));
    if (!dp) { free(parts); return 0; }
    dp[0] = 1;

    /* Process part 1 separately: it guarantees one representation of all 1's */
    for (size_t i = 1; i <= max_n; ++i) dp[i] = dp[i - 1];

    /* Process all other parts (odd prime powers > 1) */
    for (size_t idx = 1; idx < parts_cnt; ++idx) {  /* skip part 1 at index 0 */
        uint64_t p = parts[idx];
        for (size_t i = p; i <= max_n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[max_n];
    free(dp);
    free(parts);
    return result;
}
