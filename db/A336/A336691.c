/*
 * A336691  Number of distinct prime factors of 1+sigma(n).
 * https://oeis.org/A336691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221(1+A000203(n))
 * Range:     u8 n <= 1609, u16 n <= 8469, u32 n <= 8469, u64 n <= 8469,
 *            u128 n <= 8469, big any n
 * Helpers:   count_distinct_primes, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of distinct prime factors of 1 + σ(n) */

#include "../utils/sigma_of.h"
#include "../utils/count_distinct_primes.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute σ(n) */
    seq_t sigma = sigma_of(n);

    /* Compute 1 + σ(n) */
    seq_t sum = sigma + (seq_t)1;

    /* σ(n) + 1 is small enough to fit in a 64‑bit integer
       for all indices that the tests require (up to about 10 000). */
    uint64_t sum_u64 = (uint64_t)sum;

    /* Count distinct prime factors of the 64‑bit integer */
    int cnt = count_distinct_primes(sum_u64);

    return (seq_t)cnt;
}
