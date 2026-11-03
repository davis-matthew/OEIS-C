/*
 * A301315  Multiplicative with a(p^e) = prime(p) ^ a(e) (where prime(k)
 *          denotes the k-th prime number).
 * https://oeis.org/A301315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 161,
 *            u128 n <= 255, big any n
 * Validated: standard, pass, 66 terms through n = 9241 (2026-11-03)
 * Helpers:   factor_u64, ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative: a(p^e) = prime(p)^a(e),  a(1) = 1 */

/* Helper functions from the utilities library. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Static cache for memoisation.  cache[0] is unused; 1‑based indexing. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Base case.  Cache the value for 1 to avoid repeated recursion. */
    if (n == 1) {
        ensure_cache(1, &cache, &cache_cap);
        cache[1] = (seq_t)1;
        return (seq_t)1;
    }

    /* Make sure the cache is large enough for index n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Return the value if it has been computed earlier. */
    if (cache[n] != 0)
        return cache[n];

    /* Factorise n: acquire prime factors and exponents. */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Compute the multiplicative value from the factorisation. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        /* Fetch the (p‑th) prime: p is a prime number itself. */
        seq_t base = (seq_t)get_prime((idx_t)pr[i]);

        /* Recursively compute a(e). */
        seq_t exp = a((idx_t)ex[i]);

        /* Multiply by base^exp. */
        seq_t power = ipow_idx(base, (idx_t)exp);
        result = seq_mul(result, power);
    }

    /* Store the result and return it. */
    cache[n] = result;
    return result;
}
