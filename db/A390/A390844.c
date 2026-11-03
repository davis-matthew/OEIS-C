/*
 * A390844  7-valuation of Product_{1 <= k <= n} (prime(k) + 1) / (prime(k+1)
 *          - prime(k)).
 * https://oeis.org/A390844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v7( ∏_{k=1..n} * (prime(k)+1)/(prime(k+1)-prime(k)) )
 * Range:     u8 n <= 1810, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9806 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Exact exponent of 7 dividing an unsigned 64‑bit integer. */
static inline seq_t v7_uint64(uint64_t x) {
    seq_t e = 0;
    while (x % 7ULL == 0ULL) {
        ++e;
        x /= 7ULL;
    }
    return e;
}

/* Caches the computed values so that calls in arbitrary order are
 * handled efficiently.  cache_len is the number of valid entries
 * currently in *cache (cache_len == index+1 for the largest
 * calculated index). */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Ensure the cache can hold all indices up to n (inclusive). */
static void ensure_cache(idx_t n) {
    if (cache == NULL) {
        cache_cap = 4;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) abort();
        cache[0] = (seq_t)0;   /* a(0) = 0 */
        cache_len = 1;
    }
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Computes a(n).  The function may be called in any order. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Compute any missing entries up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        uint32_t pk  = get_prime((idx_t)i);          /* prime(i)   */
        uint32_t pk1 = get_prime((idx_t)(i + 1));    /* prime(i+1) */

        seq_t num = v7_uint64((uint64_t)pk + 1ULL);     /* v7(p_i+1)   */
        seq_t den = v7_uint64((uint64_t)(pk1 - pk));    /* v7(p_{i+1}-p_i) */

        int64_t delta = (int64_t)num - (int64_t)den;
        int64_t val = (int64_t)prev + delta;
        cache[i] = (seq_t)val;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
