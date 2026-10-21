/*
 * A286222  Number of partitions of n into distinct parts with an odd number
 *          of prime divisors (counted with multiplicity).
 * https://oeis.org/A286222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts where each
 *            part has an odd number of prime divisors counted with
 *            multiplicity (i.e. ω(part) is odd)
 * Range:     u8 n <= 58, u16 n <= 152, u32 n <= 457, u64 n <= 1523,
 *            u128 n <= 5451, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the count for any n >= 0.
   It uses a simple subset‑sum DP over the set of integers
   with odd ω, which contains roughly half of all integers. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* for build_primes_upto */

seq_t a(idx_t n) {
    if (n == 0) return 1;   /* a(0)=1 by definition */

    /* Allocate array for ω(i) (number of prime factors counted with multiplicity) */
    uint8_t *omega = (uint8_t*)malloc((n + 1) * sizeof(uint8_t));
    if (!omega) return 0;   /* allocation failure -> 0 count */
    omega[0] = 0;
    omega[1] = 0;

    /* Get list of primes up to n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Compute ω(i) for all i <= n */
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t m = i;
        uint8_t cnt = 0;
        for (size_t j = 0; j < prime_cnt && primes[j] * primes[j] <= m; ++j) {
            uint64_t p = primes[j];
            while (m % p == 0) {
                ++cnt;
                m /= p;
            }
        }
        if (m > 1) ++cnt;  /* remaining prime factor */
        omega[i] = cnt;
    }

    /* Small DP array to store partition counts */
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!dp) {
        free(primes);
        free(omega);
        return 0;
    }
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;  /* empty partition */

    /* Typical 0/1 knapsack over parts with odd ω */
    for (idx_t part = 2; part <= n; ++part) {
        if ((omega[part] & 1) == 0) continue;   /* skip even ω */
        for (idx_t i = n; i >= part; --i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];

    /* Clean up */
    free(dp);
    free(primes);
    free(omega);

    return result;
}
