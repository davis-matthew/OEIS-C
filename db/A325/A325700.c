/*
 * A325700  Numbers with as many distinct even as distinct odd prime indices.
 * https://oeis.org/A325700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A325700: Numbers with as many distinct even as distinct odd prime indices.
   This implementation scans the integers, tests the property by prime
   factorisation, and memoises the results. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_array = NULL;   /* list of primes */
static size_t   prime_count = 0;       /* number of primes in prime_array */
static size_t   prime_limit  = 0;      /* maximum prime value in prime_array */

static seq_t *cache = NULL;      /* memoised sequence values */
static size_t cache_len = 0;     /* how many values are stored */
static size_t cache_cap = 0;     /* capacity of cache array */
static uint64_t current_number = 1; /* next integer to test */

/* Extend the prime list so that it contains all primes <= val. */
static void ensure_primes_at_least(uint64_t val)
{
    if (val <= prime_limit) return;
    size_t new_limit = (size_t)val;
    if (new_limit < 1000) new_limit = 1000;
    /* built anew – free the old array */
    if (prime_array) free(prime_array);
    /* build_primes_upto allocates and returns the array */
    build_primes_upto(new_limit, &prime_array, &prime_count);
    prime_limit = new_limit;
}

/* Binary search: return 0‑based index of prime p in prime_array. */
static size_t prime_index_uint64(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_array[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* p is prime and must be present */
    return lo;
}

/* Test whether x has an equal number of distinct odd and distinct even
   prime indices. */
static bool has_property(uint64_t x)
{
    if (x == 1) return true;

    uint64_t pr[64], ex[64];
    size_t   cnt;
    factor_u64(x, pr, ex, &cnt);

    /* find the largest prime factor to extend our prime table if needed */
    uint64_t maxp = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (pr[i] > maxp) maxp = pr[i];
    ensure_primes_at_least(maxp);

    uint32_t odd_idx[64], even_idx[64];
    size_t   oddcnt = 0, evencnt = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint32_t idx = (uint32_t)(prime_index_uint64(pr[i]) + 1); /* 1‑based */

        if (idx & 1) { /* odd index */
            bool seen = false;
            for (size_t j = 0; j < oddcnt; ++j)
                if (odd_idx[j] == idx) { seen = true; break; }
            if (!seen) odd_idx[oddcnt++] = idx;
        } else { /* even index */
            bool seen = false;
            for (size_t j = 0; j < evencnt; ++j)
                if (even_idx[j] == idx) { seen = true; break; }
            if (!seen) even_idx[evencnt++] = idx;
        }
    }
    return oddcnt == evencnt;
}

/* Ensure that cache contains at least n values. */
static void ensure_cache(idx_t n)
{
    if (!cache) {
        /* initialise prime list with a reasonable bound */
        size_t init_limit = 200000;
        build_primes_upto(init_limit, &prime_array, &prime_count);
        prime_limit = init_limit;

        cache_cap = 64;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    }

    while (cache_len < (size_t)n) {
        if (has_property(current_number)) {
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)current_number;
        }
        ++current_number;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n - 1];
}
