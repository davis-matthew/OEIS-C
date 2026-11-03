/*
 * A390842  3-valuation of Product_{k=1..n} (prime(k) + 1) / (prime(k+1) -
 *          prime(k)).
 * https://oeis.org/A390842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (v₃(prime(k)+1) – v₃(prime(k+1)‑prime(k)))
 * Range:     u8 n <= 703, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9547 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where v₃(m) is the exponent of 3 in the factorisation of m. */

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

/* Compute the exponent of 3 in the factorisation of a 32‑bit integer. */
static int v3_uint32(uint32_t x)
{
    int cnt = 0;
    while (x % 3U == 0U && x != 0U) {
        ++cnt;
        x /= 3U;
    }
    return cnt;
}

/* The build supplies these types and helper functions */
extern idx_t _floor_isqrt_u64(idx_t n);

/* Static storage for memoization */
static seq_t *a_cache = NULL;   /* array of computed a(n) values */
static size_t a_cap   = 0;      /* current capacity of a_cache */
static idx_t  max_cached = (idx_t)-1;   /* largest n for which a_cache[n] is valid */

/* Ensure the cache can hold indices up to 'needed' (inclusive). */
static void grow_cache(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
    if (!tmp) exit(1);            /* out of memory */
    a_cache = tmp;
    a_cap   = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0U) return 0U;         /* a(0) = 0 by definition */

    /* Initialise cache on first call */
    if (a_cache == NULL) {
        grow_cache(2);
        a_cache[0] = 0U;            /* a(0) */
        max_cached  = 0U;
    }

    if (n > max_cached) {
        /* Extend cache to hold value for n */
        grow_cache((size_t)n + 1);

        /* Compute successive values from max_cached+1 up to n */
        for (idx_t k = max_cached + 1U; k <= n; ++k) {
            uint32_t pk  = get_prime(k);       /* k-th prime, 1‑based */
            uint32_t pk1 = get_prime(k + 1);   /* (k+1)-th prime */

            int vnum = v3_uint32(pk + 1U);
            int vden = v3_uint32(pk1 - pk);

            int64_t val = (int64_t)a_cache[(size_t)k - 1U] + (int64_t)vnum - (int64_t)vden;
            a_cache[(size_t)k] = (seq_t)val;
        }
        max_cached = n;
    }

    return a_cache[(size_t)n];
}
