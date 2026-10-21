/*
 * A283270  Smallest b-a such that a < prime(n)^3 < b, where a,b are
 *          semiprimes.
 * https://oeis.org/A283270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest difference b - a where a < prime(n)^3 < b and
 *            a,b are semiprimes (products of exactly two primes, counting
 *            multiplicity)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/factor_u64.h"  /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

/* Test whether an integer is a semiprime: product of exactly two primes. */
static inline bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;            /* 4 = 2*2 is the smallest semiprime */

    /* factor_u64 fills arrays with prime factors and exponents. */
    uint64_t primes[16], exps[16];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    /* Count total multiplicity. */
    size_t total = 0;
    for (size_t i = 0; i < cnt; ++i) total += exps[i];

    return total == 2;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n is 1-based in OEIS; get_prime(1) returns the first prime (2). */
    uint64_t p = (uint64_t)get_prime(n);
    uint64_t target = p * p * p;   /* prime^3 fits in 64 bits for n <= 10 000 */

    /* Search for the greatest semiprime below target. */
    uint64_t low = target - 1;
    while (!is_semiprime(low)) --low;

    /* Search for the smallest semiprime above target. */
    uint64_t high = target + 1;
    while (!is_semiprime(high)) ++high;

    /* Return the minimal difference. */
    return (seq_t)(high - low);
}
