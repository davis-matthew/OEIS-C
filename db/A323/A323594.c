/*
 * A323594  Primes p such that 3 is a primitive root modulo p while 27 is not.
 * https://oeis.org/A323594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 993, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323594: primes p such that p ≡ 1 (mod 3) and 3 is a primitive
 * root modulo p.  This implementation builds a prime table once and
 * caches the first n terms for arbitrary non‑increasing accesses.
 *
 * The algorithm:
 *  1. Ensure a prime table up to a fixed large limit (5 000 000) is built.
 *  2. Scan primes in increasing order, testing the two conditions.
 *     For a prime p, factor p−1 using the prime table.
 *  3. 3 is a primitive root iff for every distinct prime divisor q of
 *     p−1, 3^((p−1)/q) mod p ≠ 1.
 *  4. Store each qualifying p in a cache; return a[n−1] from that cache.
 *
 * This uses only the permitted utilities: build_primes_upto, mod_pow,
 * and standard library functions.  It is valid C11 and C++17. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"

/* static cache for the sequence values */
static seq_t *seq_cache      = NULL;
static size_t seq_cache_len  = 0;
static size_t seq_cache_cap  = 0;

/* static prime table used for factorisation */
static uint64_t *prime_tbl    = NULL;
static size_t    prime_cnt    = 0;
static bool      primes_built = false;

/* buffer for distinct prime factors of p−1 */
static uint64_t *factor_buf   = NULL;
static size_t    factor_cap   = 0;

/* index of next prime to examine in prime_tbl */
static size_t prime_iter = 0;

/* initialise the prime table and factor buffer once */
static void
init_prime_table(void)
{
    if (primes_built)
        return;

    /* 5 000 000 is more than enough for n up to 10 000
     * (≈ 6 000 more than needed for the first 10 000 terms). */
    size_t cnt = 0;
    build_primes_upto(5000000ULL, &prime_tbl, &cnt);
    prime_cnt = cnt;
    primes_built = true;

    /* Allocate a buffer that can hold all primes as distinct factors.
     * This size is safe because the number of distinct prime divisors
     * of p−1 never exceeds log₂(p−1). */
    factor_buf = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    factor_cap = prime_cnt;
}

/* append a value to the sequence cache, growing the array if needed */
static void
cache_append(seq_t val)
{
    if (seq_cache_len == seq_cache_cap) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(seq_cache,
                                      new_cap * sizeof(seq_t));
        if (!tmp)
            abort();          /* unrecoverable memory failure */
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }
    seq_cache[seq_cache_len++] = val;
}

/* main sequence function, supporting arbitrary order of calls */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but safe guard */

    init_prime_table();

    /* extend the cache until it contains at least n terms */
    while (seq_cache_len < n) {
        if (prime_iter >= prime_cnt) {
            /* safety net: we ran out of primes; grow the table */
            size_t new_limit = prime_tbl[prime_cnt - 1] * 2;
            /* free the old table (the build function allocates new memory) */
            free(prime_tbl);
            build_primes_upto(new_limit, &prime_tbl, &prime_cnt);
            /* factor buffer remains the same size */
            /* restart scanning from the beginning */
            prime_iter = 0;
        }

        uint64_t p = prime_tbl[prime_iter++];
        if (p < 3 || (p % 3) != 1)
            continue;            /* skip non‑1 (mod 3) primes */

        /* factor p−1, collecting distinct prime divisors */
        uint64_t temp = p - 1;
        size_t nf = 0;
        for (size_t i = 0; i < prime_cnt && prime_tbl[i] * prime_tbl[i] <= temp; ++i) {
            uint64_t d = prime_tbl[i];
            if (temp % d == 0) {
                factor_buf[nf++] = d;
                while (temp % d == 0)
                    temp /= d;
            }
        }
        if (temp > 1)
            factor_buf[nf++] = temp;

        /* test that 3 is a primitive root modulo p */
        bool primitive = true;
        for (size_t i = 0; i < nf; ++i) {
            uint64_t q = factor_buf[i];
            uint64_t exp = (p - 1) / q;
            if (mod_pow((seq_t)3, exp, (seq_t)p) == (seq_t)1) {
                primitive = false;
                break;
            }
        }
        if (primitive)
            cache_append((seq_t)p);
    }

    return seq_cache[n - 1];
}
