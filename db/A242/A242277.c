/*
 * A242277  Least positive primitive root g < prime(n) mod prime(n) such that
 *          g is the sum of the first k primes for some k > 0, or 0 if such a
 *          number g does not exist.
 * https://oeis.org/A242277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive primitive root g < prime(n) such that g is
 *            the sum of the first k primes for some k > 0, or 0 if no such g
 *            exists
 * Range:     u8 n <= 54, u16 n <= 7015, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto(...)  */
#include "../utils/mod_pow.h"           /* mod_pow(seq_t, uint64_t, seq_t) */

static uint64_t *prime_list = NULL;   /* primes as uint64_t for safety   */
static size_t prime_count = 0;        /* number of primes available      */

/* Ensure that prime_list contains at least n primes (prime(n) exists) */
static void ensure_primes(idx_t n)
{
    if (prime_count >= n)
        return;

    /* An upper bound for the nth prime (for n ≥ 1, n*20 is safe up to n≈10000) */
    idx_t limit = n * 20 + 100;       /* safety margin                */
    while (1) {
        if (prime_list) {
            free(prime_list);
            prime_list = NULL;
        }
        build_primes_upto(limit, &prime_list, &prime_count);
        if (prime_count >= n)
            break;                   /* we have enough primes   */
        limit *= 2;                  /* grow and try again      */
    }
}

/* Factorise n (seq_t) into its distinct prime factors.
   The resulting primes are stored in `factors`; the number of distinct
   factors is written to `cnt`.  The array `factors` must be large enough. */
static void factor_distinct_seq(seq_t n, uint32_t *factors, size_t *cnt)
{
    size_t c = 0;
    uint64_t temp = (uint64_t)n;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = (uint64_t)prime_list[i];
        if (p * p > temp)
            break;
        if (temp % p == 0) {
            factors[c++] = (uint32_t)p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)
        factors[c++] = (uint32_t)temp;   /* remaining prime factor */
    *cnt = c;
}

/* Test whether g is a primitive root modulo the prime p.
   `factors` contains the distinct prime factors of p-1. */
static bool is_primitive_root(seq_t g, seq_t p, const uint32_t *factors, size_t fc)
{
    seq_t p_minus1 = p - 1;
    for (size_t i = 0; i < fc; ++i) {
        uint64_t exp = (uint64_t)(p_minus1 / (seq_t)factors[i]);
        if (mod_pow(g, exp, p) == 1)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* not used, but harmless    */

    ensure_primes(n);

    seq_t p = (seq_t)prime_list[n - 1];   /* prime(n) is 1‑based           */
    if (p == 2)                          /* prime(1) = 2 has no g < p    */
        return (seq_t)0;

    /* Factorise p‑1 once */
    uint32_t factors[64];
    size_t factor_count;
    factor_distinct_seq(p - 1, factors, &factor_count);

    /* Sum the first primes one by one until reaching or exceeding p */
    uint64_t sum64 = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t pr = (uint64_t)prime_list[i];
        sum64 += pr;
        if (sum64 >= (uint64_t)p)
            break;                       /* g must be < p              */
        seq_t g = (seq_t)sum64;          /* safe: sum64 < p ≤ seq_t max */
        if (is_primitive_root(g, p, factors, factor_count))
            return g;
    }
    return (seq_t)0;                     /* no suitable primitive root  */
}
