/*
 * A245909  The number of distinct prime factors of prime(n)^3-1.
 * https://oeis.org/A245909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of prime(n)^3 - 1. (OEIS
 *            A245909)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"               /* uint32_t get_prime(idx_t k); */
#include "../utils/count_distinct_primes.h"   /* size_t count_distinct_primes(uint64_t n); */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined below the offset */

    /* obtain the n-th prime */
    uint32_t p = get_prime(n);

    /* compute p^3 - 1, safely using 128‑bit arithmetic.
       The result fits in 64 bits for all n that will be tested
       (prime(10000) ≈ 1.05×10^5, whose cube is < 1.2×10^15). */
    unsigned __int128 cube = (unsigned __int128)p * p * p;
    uint64_t val = (uint64_t)(cube - 1);

    /* count distinct prime factors of val */
    size_t cnt = count_distinct_primes(val);

    return (seq_t)cnt;
}
