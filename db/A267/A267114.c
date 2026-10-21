/*
 * A267114  Numbers n for which A001222(n) = A267115(n) + A267116(n).
 * https://oeis.org/A267114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A267114 – numbers n for which A001222(n) = A267115(n)+A267116(n).
   The function tests each integer with a linear sieve and caches the
   values it finds.  It runs in O(n log log n) time. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global sieve state – the arrays are grown as needed. */
static uint64_t *spf = NULL;       /* smallest prime factor table */
static size_t   spf_cap = 0;       /* current allocated size */

static uint64_t *primes = NULL;    /* list of primes found so far */
static size_t   primes_len = 0;
static size_t   primes_cap = 0;

/* Ensure spf and primes tables are computed up to at least 'limit'. */
static void extend_sieve(uint64_t limit)
{
    if (limit + 1 <= spf_cap)           /* already big enough */
        return;

    size_t new_cap = spf_cap ? spf_cap << 1 : 64;
    while (new_cap <= limit + 1)
        new_cap <<= 1;

    spf = (uint64_t*)realloc(spf, new_cap * sizeof(uint64_t));
    if (!spf) abort();

    for (size_t i = spf_cap; i < new_cap; ++i)
        spf[i] = 0;
    spf_cap = new_cap;

    /* Linear sieve – recompute the whole table.  The work is small
       because the capacity grows geometrically. */
    for (size_t i = 2; i < spf_cap; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap << 1 : 8;
                primes = (uint64_t*)realloc(primes,
                                            primes_cap * sizeof(uint64_t));
                if (!primes) abort();
            }
            primes[primes_len++] = i;
        }
        for (size_t j = 0; j < primes_len; ++j) {
            uint64_t p = primes[j];
            uint64_t v = i * p;
            if (v >= spf_cap) break;
            if (p > spf[i]) break;
            spf[v] = p;
        }
    }
}

/* Test whether given x satisfies the defining condition. */
static bool satisfies(uint64_t x)
{
    if (x == 1)           /* 1 is included by definition */
        return true;

    uint64_t temp = x;
    uint64_t or_val = 0;
    uint64_t and_val = 0;
    bool first = true;
    uint64_t sum = 0;

    while (temp > 1) {
        uint64_t p   = spf[temp];
        uint64_t cnt = 0;
        while (temp % p == 0) {
            temp /= p;
            ++cnt;
        }
        sum += cnt;
        or_val |= cnt;
        if (first) {
            and_val = cnt;
            first   = false;
        } else {
            and_val &= cnt;
        }
    }
    return sum == or_val + and_val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static bool      initialized = false;
    static seq_t    *cache       = NULL;
    static size_t    cache_len   = 0;
    static size_t    cache_cap   = 0;
    static uint64_t  next        = 2;      /* next integer to test */

    if (!initialized) {
        /* 1 is the first term. */
        cache_len   = 1;
        cache_cap   = 8;
        cache       = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        next = 2;
        extend_sieve(64);                  /* some initial sieve */
        initialized = true;
    }

    while (cache_len < n) {
        if (!satisfies(next))
            goto skip;                      /* don't add to cache */

        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            seq_t *new_cache = (seq_t*)realloc(cache,
                                               cache_cap * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;
        }
        cache[cache_len++] = (seq_t)next;

    skip:
        ++next;
        if (next >= spf_cap)
            extend_sieve(next * 2);          /* grow sieve as needed */
    }

    return cache[n - 1];
}
