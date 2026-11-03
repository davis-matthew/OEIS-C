/*
 * A323739  a(n) is the number of residues modulo (4*primorial(n)) of the
 *          squares of primes greater than or equal to prime(n+1).
 * https://oeis.org/A323739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * ∏_{k=1..n} (prime(k)-1) / 2 Recurrence: a(0) = 2; for
 *            n≥1: a(n) = a(n-1) * (prime(n)-1) / 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 168 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
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

/* Compute the n-th term of A323739 */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = 0;   /* last computed index (>=0) */

    /* Initialise cache for a(0) on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return 0;                 /* unlikely */
        cache[0] = (seq_t)2;                  /* a(0) = 2 */
        cache_last = 0;
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0;                       /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t p = (seq_t)get_prime(i);           /* prime(i) */
        seq_t prod = cache[i - 1] * (p - (seq_t)1);
        cache[i] = prod / (seq_t)2;              /* exact division */
    }
    cache_last = n;

    return cache[n];
}
