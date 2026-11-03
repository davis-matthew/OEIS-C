/*
 * A285298  Number of 10's found in the first differences of a reduced residue
 *          system modulo a primorial p#.
 * https://oeis.org/A285298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*Π(p-2) - 6*Π(p-3) + 2*Π(p-4) with the product over all
 *            primes p > 5 and p ≤ prime(n). For n = 1,2,3 the product is
 *            empty and a(n) = 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* Memoisation state */
    static seq_t *cache        = NULL;   /* cache[n-1] holds a(n)       */
    static size_t cache_cap    = 0;      /* allocated size (terms)      */
    static idx_t  max_computed = 0;      /* highest n already computed  */
    static seq_t prod2 = 1, prod3 = 1, prod4 = 1;  /* running products     */

    if (n == 0)            /* safety guard – undefined, return 0 */
        return (seq_t)0;

    /* Ensure enough space in the cache */
    if (cache_cap < n) {
        size_t new_cap = n > 2 ? n : 2;               /* avoid zero size   */
        seq_t *tmp = mem_resize(seq_t, cache, (new_cap * sizeof *tmp) / sizeof(seq_t));
        if (!tmp) return (seq_t)0;                    /* allocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from max_computed+1 up to n */
    for (idx_t i = max_computed; i < n; ++i) {
        idx_t idx = i + 1;                           /* 1‑based index      */
        uint32_t p = get_prime(idx);                 /* prime at position  */

        /* Only primes > 5 contribute to the products */
        if (p > 5) {
            prod2 *= (seq_t)(p - 2);
            prod3 *= (seq_t)(p - 3);
            prod4 *= (seq_t)(p - 4);
        }

        /* a(n) = 4*prod2 - 6*prod3 + 2*prod4 */
        seq_t val = prod2 * (seq_t)4            /* 4*prod2 */
                 - prod3 * (seq_t)6            /* 6*prod3 (subtracted) */
                 + prod4 * (seq_t)2;           /* 2*prod4          */

        cache[i] = val;
    }

    max_computed = n;
    return cache[n - 1];
}
