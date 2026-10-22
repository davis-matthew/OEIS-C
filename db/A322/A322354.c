/*
 * A322354  Greatest common divisor of product p and product (p+2), where p
 *          ranges over distinct prime divisors of n; a(n) = gcd(A007947(n),
 *          A166590(A007947(n))).
 * https://oeis.org/A322354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(rad(n), ∏_{p|n}(p+2)) where rad(n) = ∏_{p|n} p
 *            (product of distinct prime factors)
 * Range:     u8 n <= 77, u16 n <= 9827, u32 n <= 9827, u64 n <= 9827,
 *            u128 n <= 9827, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "./../utils/gcd.h"
#include "./../utils/int_sqrt.h"
#include "./../utils/build_primes_upto.h"
#include "./../utils/seq_mul.h"

/* static cache for primes up to the largest limit needed so far */
static uint64_t *prime_cache = NULL;  /* array of primes */
static size_t prime_cnt = 0;          /* number of primes in the cache */
static idx_t  prime_limit = 0;        /* maximum sqrt value cached */

/* Ensure that we have primes up to at least `limit`.  Return true on success. */
static bool ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return true;                /* already have enough */

    /* free old cache and build a new one up to `limit` */
    if (prime_cache != NULL)
        free(prime_cache);

    build_primes_upto(limit, &prime_cache, &prime_cnt);
    if (prime_cache == NULL || prime_cnt == 0)
        return false;               /* failed */

    prime_limit = limit;
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Factor n into distinct prime factors using the cached prime list */
    uint64_t temp = (uint64_t)n;
    idx_t     sqrt_n = int_sqrt(n);            /* sqrt of original n */
    ensure_primes_upto(sqrt_n);                /* prime list up to sqrt_n */

    seq_t rad  = (seq_t)1;     /* rad(n) */
    seq_t prod = (seq_t)1;     /* ∏ (p+2) */

    /* iterate over primes <= sqrt(temp) */
    for (size_t i = 0; i < prime_cnt && prime_cache[i] * (uint64_t)prime_cache[i] <= temp; ++i) {
        uint64_t p = prime_cache[i];
        if (temp % p == 0) {
            rad  = seq_mul(rad,  (seq_t)p);
            prod = seq_mul(prod, (seq_t)(p + 2));

            /* remove all copies of p from temp */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* if temp > 1 it is a remaining prime factor larger than sqrt(n) */
    if (temp > 1) {
        rad  = seq_mul(rad,  (seq_t)temp);
        prod = seq_mul(prod, (seq_t)(temp + 2));
    }

    /* return gcd(rad, prod) */
    return gcd(rad, prod);
}
