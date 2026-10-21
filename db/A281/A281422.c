/*
 * A281422  Expansion of 1/(1 - Sum_{k>=1} x^prime(prime(k))).
 * https://oeis.org/A281422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into primes of the form
 *            prime(prime(k))
 * Range:     u8 n <= 32, u16 n <= 59, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* generate all primes up to n */
    uint64_t *all_primes = NULL;
    size_t all_cnt = 0;
    build_primes_upto(n, &all_primes, &all_cnt);

    /* collect the allowed parts: primes whose index (1‑based) is prime */
    size_t *parts = (size_t*)malloc(all_cnt * sizeof(size_t));
    size_t part_cnt = 0;
    for (size_t k = 1; k <= all_cnt; ++k) {
        if (is_prime((idx_t)k)) {
            uint64_t v = all_primes[k - 1];
            if ((idx_t)v <= n) {
                parts[part_cnt++] = (size_t)v;
            }
        }
    }

    /* dynamic programming for compositions */
    seq_t *dp = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j < part_cnt; ++j) {
            size_t p = parts[j];
            if ((idx_t)p > i) break;
            size_t idx = (size_t)(i - (idx_t)p);
            sum += dp[idx];
        }
        dp[i] = sum;
    }

    seq_t result = dp[n];
    free(dp);
    free(parts);
    free(all_primes);
    return result;
}
