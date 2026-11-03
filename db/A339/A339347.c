/*
 * A339347  Primes p such that p < (gpf((p - 1)/gpf(p - 1)))^4, where gpf(k)
 *          is the greatest prime factor of k, A006530.
 * https://oeis.org/A339347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1841, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 8972 (2026-11-03)
 * Helpers:   is_prime, largest_prime_factor, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime p such that p < (gpf((p-1)/gpf(p-1)))^4.
   The sequence is generated lazily and cached.  The cache is expanded
   only as needed, allowing calls to a(n) in arbitrary order.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/largest_prime_factor.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;            /* array of qualifying primes */
static size_t cache_len = 0;           /* number of primes stored */
static size_t cache_cap = 0;           /* allocated capacity */
static uint64_t last_checked = 2;      /* last integer examined for primality */

/* Ensure the cache can hold at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = mem_new(seq_t, new_cap);
    if (!tmp) {
        tmp = mem_new(seq_t, need);
        if (!tmp) exit(EXIT_FAILURE);
    }
    if (cache) {
        mem_copy(tmp, cache, cache_len * sizeof(seq_t));
        mem_free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a new qualifying prime to the cache */
static void cache_append(uint64_t p)
{
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len] = (seq_t)p;
    cache_len++;
}

/* Generate qualifying primes until cache_len >= n */
static void grow_cache_to(idx_t n)
{
    while (cache_len < n) {
        /* Find next prime > last_checked */
        uint64_t cand = 0;
        do {
            last_checked++;
            if (last_checked % 2 == 0)
                last_checked++;          /* skip even numbers */
        } while (!is_prime((idx_t)last_checked));
        cand = last_checked;

        /* Test the condition */
        uint64_t q = cand - 1;
        uint64_t g1 = largest_prime_factor(q);
        uint64_t t  = q / g1;
        uint64_t g2 = largest_prime_factor(t);

        /* Compute g2^4 with 128‑bit arithmetic */
        unsigned __int128 pow4 = (unsigned __int128)g2 * g2 * g2 * g2;
        if ((unsigned __int128)cand < pow4)
            cache_append(cand);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for 0, but harmless */
    grow_cache_to(n);
    return cache[n - 1];
}
