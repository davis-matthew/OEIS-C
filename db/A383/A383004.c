/*
 * A383004  Exponent of the highest power of 2 dividing the n-th cubefree
 *          number.
 * https://oeis.org/A383004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root, two_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes exponent of the highest power of 2 dividing the n‑th cubefree number
   (OEIS A383004).  The other index and result types are supplied by the build
   system.  The code uses a simple sieve for primes up to the cube root of the
   current candidate and generates cubefree numbers incrementally, storing
   the 2‑adic valuation of each.  The cache is thread‑unsafe but is safe
   for single‑threaded use. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/int_cube_root.h"
#include "../utils/two_exp.h"

/* Cache of 2‑adic valuations of the cubefree numbers. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of cached terms   */
static size_t cache_cap = 0;   /* allocated capacity       */

/* Prime list used for cubefree testing. */
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;
static uint32_t prime_lim = 0;

/* The next natural number to test for cubefreeness. */
static uint64_t last_num = 1;

/* Simple sieve to fill primes[0..prime_cnt-1] up to 'limit'. */
static void ensure_primes(uint32_t limit)
{
    if (limit <= prime_lim)
        return;

    /* Allocate new primes array if necessary. */
    size_t required = limit + 1;
    uint32_t *tmp = (uint32_t *)realloc(primes, required * sizeof *primes);
    if (!tmp) abort();
    primes = tmp;

    /* Sieve of Eratosthenes. */
    uint8_t *mark = (uint8_t *)calloc(required, 1);
    if (!mark) abort();

    size_t cnt = 0;
    for (uint32_t p = 2; p <= limit; ++p) {
        if (!mark[p]) {
            primes[cnt++] = p;
            if ((uint64_t)p * p <= limit) {
                for (uint64_t q = (uint64_t)p * p; q <= limit; q += p)
                    mark[(size_t)q] = 1;
            }
        }
    }
    free(mark);
    prime_cnt = cnt;
    prime_lim = limit;
}

/* Test whether x is cubefree: no prime factor with exponent >= 3. */
static bool is_cubefree(uint64_t x)
{
    uint64_t y = x;
    if (y < 2)
        return true;

    /* Ensure primes cover up to cube root of y. */
    uint64_t lim = int_cube_root(y) + 1;
    if (lim > prime_lim)
        ensure_primes((uint32_t)lim);

    for (size_t i = 0; i < prime_cnt &&
         (uint64_t)primes[i] * primes[i] * primes[i] <= y; ++i) {
        uint32_t p = primes[i];
        int cnt = 0;
        while (y % p == 0) {
            y /= p;
            ++cnt;
            if (cnt >= 3)
                return false;
        }
    }
    return true;
}

/* Grow the cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (n <= (idx_t)cache_len)
        return;

    /* Grow cache array if necessary. */
    if (cache_len + (size_t)n > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        while (new_cap < cache_len + (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof *cache);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate cubefree numbers until we have n terms. */
    for (; cache_len < (size_t)n; ++last_num) {
        if (is_cubefree(last_num))
            cache[cache_len++] = two_exp(last_num);
    }
}

/* Public entry point.  idx_t n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for 0, but safe fallback */
        return 0;
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
