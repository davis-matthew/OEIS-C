/*
 * A390440  Exponentially 5-rough numbers: numbers whose prime factorization
 *          exponents are all coprime to 6.
 * https://oeis.org/A390440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all positive integers n such that every
   exponent in the prime factorization of n is coprime to 6.
   Equivalently, for each prime p we have n = Π p^e with e ≡ 1 or 5 (mod 6). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

static bool initialized = false;
static idx_t sieve_limit = 200000;          /* enough for n up to ~10000   */
static idx_t *spf = NULL;                   /* smallest prime factor table */
static seq_t *cache = NULL;                 /* memoisation of results     */
static size_t cache_size = 0;               /* number of cached values     */
static size_t cache_cap = 0;                /* allocated capacity          */
static idx_t current = 1;                   /* next integer to test        */

/* initialise static structures on first use */
static void init(void)
{
    if (initialized) return;

    /* build SPF table up to sieve_limit */
    build_spf(sieve_limit, &spf);

    /* start with a modest cache capacity */
    cache_cap = 1024;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    cache_size = 0;

    initialized = true;
}

/* test whether number x has all exponents ≡ 1 or 5 mod 6 */
static bool good_exponents(idx_t x)
{
    if (x == 1) return true;          /* 1 has no prime factors */

    while (x > 1) {
        idx_t p = spf[x];
        idx_t e = 0;
        while (x % p == 0) {
            x /= p;
            e++;
        }
        if (e % 6 != 1 && e % 6 != 5) return false;
    }
    return true;
}

/* ensure cache covers at least n elements, extending it if needed */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;   /* already computed */

    /* expand cache array */
    while (cache_size < n) {
        if (cache_size == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        if (good_exponents(current)) {
            cache[cache_size++] = (seq_t)current;
        }
        current++;
        /* if we have exceeded the sieve limit, abort: should not happen for n <= 10000 */
        if (current > sieve_limit) {
            /* Rebuild a larger SPF table if necessary */
            sieve_limit <<= 1;
            build_spf(sieve_limit, &spf);
        }
    }
}

/* public function */
seq_t a(idx_t n)
{
    init();
    if (n < 1) return 0;       /* undefined for n<1, return 0 as a safe value */

    ensure_cache(n);
    return cache[n - 1];
}
