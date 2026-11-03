/*
 * A302156  a(n) = Product_{k=1..n} prime(k+1)^(n-k+1).
 * https://oeis.org/A302156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} prime(k+1)^(n-k+1) a(0) = 1 Recurrence:
 *            a(n) = a(n-1) * (primorial(n+1) / 2) where primorial(m) =
 *            product of the first m primes
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* Static cache for computed values, capacity, and helper data. */
    static seq_t *cache          = NULL;
    static size_t cache_cap      = 0;
    static idx_t last_cached     = 0;
    static seq_t primorial       = 0; /* product of first (last_cached+1) primes */

    /* One-time initialization. */
    if (cache == NULL) {
        /* Ensure space for the first element. */
        ensure_cache(0, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        last_cached = 0;
        /* primorial(1) = product of the first 1 prime = 2. */
        primorial = (seq_t)get_prime(1);
    }

    /* Extend cache up to requested index, if necessary. */
    if (n > last_cached) {
        /* Ensure the array holds entries up to n. */
        ensure_cache(n, &cache, &cache_cap);

        for (idx_t i = last_cached + 1; i <= n; ++i) {
            /* Update primorial to product of first (i+1) primes. */
            seq_t prime_next = (seq_t)get_prime(i + 1);
            primorial *= prime_next;

            /* a(i) = a(i-1) * primorial / 2. */
            seq_t prev = cache[i - 1];
            cache[i] = (prev * primorial) / (seq_t)2;
        }
        last_cached = n;
    }

    return cache[n];
}
