/*
 * A245908  The number of distinct prime factors of prime(n)^2-1.
 * https://oeis.org/A245908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of prime(n)^2-1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑based). */
    uint32_t p = get_prime(n);

    /* Compute p^2 - 1 as a 64‑bit integer (fits for n ≤ 10 000). */
    uint64_t m = (uint64_t)p * (uint64_t)p - 1ULL;

    /* Count distinct prime factors of m. */
    int cnt = count_distinct_primes(m);

    /* Return the count as seq_t. */
    return (seq_t)cnt;
}
